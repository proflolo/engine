#include "StdAfx.h"
#if ENGINE_OPENGL
#include "RendererOpenGL.h"
#include "render/Mesh.h"
#include "render/Material.h"
#include "MeshOpenGL.h"
#include "MaterialOpenGL.h"
#include "OpenGLResult.h"
#include "asset/AssetProvider.h"

namespace engine
{
	
	RendererOpenGL::RendererOpenGL(const Context& i_context)
		: m_engineContext(i_context)
	{
		glEnable(GL_DEBUG_OUTPUT);
#if !WITH_EDITOR
		glDebugMessageCallback(DebugMessageCallback, 0);
#endif
	}

	RendererOpenGL::~RendererOpenGL()
	{
		
	}

	void RendererOpenGL::BeginRender(std::stop_token i_stopToken)
	{
		glClearColor(0.129f, 0.586f, 0.949f, 1.0f);	// rgb(33,150,243)
		glClear(GL_COLOR_BUFFER_BIT);
	}


	template<typename T, typename Params, typename Resource>
	std::future<size_t> RendererOpenGL::_Load(const RenderResource<T>& io_resource, Params i_params, std::vector<ResourceHolder<Resource, Params>>& io_resources)
	{
		using Holder = ResourceHolder<Resource, Params>;
		const void* idx = &io_resource;
		//Find if we have the resource already
		auto it = std::find_if(io_resources.begin(), io_resources.end(), [idx](const auto& i_material)
			{
				return std::visit([idx](const auto& i_resource) -> bool
					{
						using T = std::decay_t<decltype(i_resource)>;
						if constexpr (std::is_same_v<T, std::nullopt_t>)
						{
							return false;
						}
						else if constexpr (std::is_same_v<T, typename Holder::Resource>)
						{
							return i_resource.source == idx;
						}
						else if constexpr (std::is_same_v<T, typename Holder::Request>)
						{
							return i_resource.source == idx;
						}
						else
						{
							assert(false);
							//static_assert(false);
						}
					}, i_material.resource);
			});

		//If we do, return the proper future
		if (it != io_resources.end())
		{
			size_t index = std::distance(io_resources.begin(), it);
			return std::visit([index](auto& i_resource) -> std::future<size_t>
				{
					using T = std::decay_t<decltype(i_resource)>;
					if constexpr (std::is_same_v<T, std::nullopt_t>)
					{
						assert(false);
						std::promise<size_t> incorrect;
						return incorrect.get_future();
					}
					else if constexpr (std::is_same_v<T, typename ResourceHolder<Resource, Params>::Resource>)
					{
						std::promise<size_t> completed;
						completed.set_value(index);
						return completed.get_future();
					}
					else if constexpr (std::is_same_v<T, ResourceHolder<Resource, Params>::Request>)
					{
						return i_resource.promise.get_future();
					}
					else
					{
						assert(false);
						//static_assert(false);
					}
				}, it->resource);
		}
		//We don't, create a new one and prepare the request
		else
		{
			typename Holder::Request request;
			request.source = idx;
			request.params = std::move(i_params);
			std::future<size_t> result = request.promise.get_future();

			//Find an available slot
			auto itAvailable = std::find_if(io_resources.begin(), io_resources.end(), [](const auto& i_holder) {  return i_holder.IsEmpty(); });
			if (itAvailable != io_resources.end())
			{
				request.index = std::distance(io_resources.begin(), itAvailable);
				itAvailable->lastUsed = std::chrono::system_clock::now();
				itAvailable->resource = std::move(request);
				io_resource.renderResource = request.index;
				return result;
			}
			else
			{
				request.index = io_resources.size();
				io_resource.renderResource = request.index;
				io_resources.emplace_back(std::move(request), std::chrono::system_clock::now());
				return result;
			}
		}
	}

	template<typename Params, typename Resource, typename... Args>
	void RendererOpenGL::_ProcessLoads(std::vector<ResourceHolder<Resource, Params>>& io_resources, Args&... i_args)
	{
		using Holder = ResourceHolder<Resource, Params>;
		for (auto& itMaterial : io_resources)
		{
			auto& resource = itMaterial.resource;
			std::visit([&resource, &i_args...](auto& i_resource)
				{
					using T = std::decay_t<decltype(i_resource)>;
					if constexpr (std::is_same_v<T, typename Holder::Request>)
					{
						i_resource.promise.set_value(i_resource.index);
						resource.emplace<typename Holder::Resource>(i_resource.source, Resource(i_resource.params, i_args...));
					}
				},
				itMaterial.resource);
		}
	}

	template<typename T, typename Params>
	inline void RendererOpenGL::ClearUnusedResources(std::vector<ResourceHolder<T, Params>>& io_resources)
	{
		for (auto& resource : io_resources)
		{
			if (resource.IsReady())
			{
				if (resource.lastUsed + std::chrono::seconds(5) < std::chrono::system_clock::now())
				{
					resource.resource = std::nullopt;
				}
			}
		}
	}

	

	void RendererOpenGL::EndRender(std::stop_token i_stopToken)
	{
		{
			std::lock_guard guard(m_meshMutex);
			ClearUnusedResources(m_meshes);
			_ProcessLoads(m_meshes);
		}
		{
			std::lock_guard guard(m_materialMutex);
			ClearUnusedResources(m_materials);
			_ProcessLoads(m_materials, m_compiler);
		}
		
	}

	std::future<RenderResource<MaterialGeneric>::Id> RendererOpenGL::Load(const MaterialGeneric& i_material)
	{
		std::lock_guard guard(m_materialMutex);
		return _Load(i_material, GPUResourceMaterialOpenGLParams(), m_materials);
	}

	std::future<RenderResource<MeshGeneric>::Id> RendererOpenGL::Load(const MeshGeneric& i_mesh)
	{
		std::lock_guard guard(m_meshMutex);
		std::vector<float> data(i_mesh.GetData().begin(), i_mesh.GetData().end());
		return _Load(i_mesh, GPUResourceMeshOpenGLParams(std::move(data), i_mesh.GetIndices()), m_meshes);
	}

	void RendererOpenGL::Render(MeshGeneric& i_mesh, MaterialGeneric& i_material, const RenderLayout& i_layout)
	{
		std::lock_guard guardMaterials(m_materialMutex);
		std::lock_guard guardMeshes(m_meshMutex);

		if (!i_mesh.HasRenderResource() || !m_meshes[i_mesh.renderResource].IsReady() )
		{
			return;
		}

		if (!i_material.HasRenderResource() || !m_materials[i_material.renderResource].IsReady())
		{
			return;
		}


		auto& meshGPU = m_meshes[i_mesh.renderResource];
		auto& materialGPU = m_materials[i_material.renderResource];

		
		meshGPU.lastUsed = std::chrono::system_clock::now();
		materialGPU.lastUsed = std::chrono::system_clock::now();
		Render(meshGPU.GetResource(), materialGPU.GetResource(), i_layout);
		
	}

	
	

	void RendererOpenGL::Render(GPUResourceMeshOpenGL& i_mesh, GPUResourceMaterialOpenGL& i_material, const RenderLayout& i_layout)
	{
		i_material.Use();

		glBindVertexArray(i_mesh.GetVertexArray());
		glBindBuffer(GL_ARRAY_BUFFER, i_mesh.GetVertexBuffer());
		opengl_check();

		BindLayout(0, i_layout);
		

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_mesh.GetIndexBuffer());
		opengl_check();

		// Draw the triangles !
		glDrawElements(
			GL_TRIANGLES,      // mode
			i_mesh.GetIndexSize(),    // count
			GL_UNSIGNED_INT,   // type
			(void*)0           // element array buffer offset
		);

		opengl_check();

	}

	void RendererOpenGL::BindLayout(GLuint i_arrayIndex, const RenderLayout& i_layout)
	{
		glEnableVertexAttribArray(0);
		opengl_check();
		for (const auto& field : i_layout.fields)
		{
			glVertexAttribPointer(
				field.shaderLayout, // attribute 0. No particular reason for 0, but must match the layout in the shader.
				field.size,	// size
				GL_FLOAT, // type
				GL_FALSE, // normalized?
				field.stride, // stride
				(void*)field.start // array buffer offset
			);
			opengl_check();
		}
		
	}

	void RendererOpenGL::DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{

	}

}
#endif