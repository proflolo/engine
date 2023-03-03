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


	template<typename T>
	inline void RendererOpenGL::ClearUnusedResources(std::vector<ResourceHolder<T>>& io_resources)
	{
		for (auto& resource : io_resources)
		{
			if (resource.resource)
			{
				if (resource.lastUsed + std::chrono::seconds(5) < std::chrono::system_clock::now())
				{
					resource.resource = std::nullopt;
				}
			}
		}
	}

	template<typename R, typename T>
	RenderResource<R>::Id RendererOpenGL::AllocateResource(std::vector<ResourceHolder<T>>& io_resources, T&& i_resource)
	{
		io_resources.emplace_back(std::move(i_resource), std::chrono::system_clock::now());
		size_t index = io_resources.size() -1;
		return index;
	}

	void RendererOpenGL::EndRender(std::stop_token i_stopToken)
	{
		ClearUnusedResources(m_meshes);
		ClearUnusedResources(m_materials);
	}

	void RendererOpenGL::Render(MeshGeneric& i_mesh, MaterialGeneric& i_material, const RenderLayout& i_layout)
	{
		bool canDraw = true;
		if (!i_mesh.HasRenderResource() || !m_meshes[i_mesh.renderResource].resource)
		{
			//Create it!
			i_mesh.renderResource = AllocateResource<MeshGeneric>(m_meshes, GPUResourceMeshOpenGL(i_mesh));
		}

		if (!i_material.HasRenderResource() || !m_materials[i_material.renderResource].resource)
		{
			//Create it!
			if (i_material.GetMaterialLoadState() == MaterialLoadState::Unloaded)
			{
				//Request load
				canDraw = false;
				i_material.StartLoad(m_engineContext);
			}
			else if (i_material.GetMaterialLoadState() == MaterialLoadState::Ready)
			{
				//std::shared_ptr<GPUResourceHolder> handle = std::make_shared<GPUResourceHolder>(*this, std::move(gpuResource));
				i_material.renderResource = AllocateResource<MaterialGeneric>(m_materials, GPUResourceMaterialOpenGL(m_compiler));
			}
			else
			{
				canDraw = false;
			}
		}

		if (!canDraw)
		{
			return;
		}

		ResourceHolder<GPUResourceMeshOpenGL>& meshGPU = m_meshes[i_mesh.renderResource];
		ResourceHolder<GPUResourceMaterialOpenGL>& materialGPU = m_materials[i_material.renderResource];

		if (meshGPU.resource && materialGPU.resource)
		{
			meshGPU.lastUsed = std::chrono::system_clock::now();
			materialGPU.lastUsed = std::chrono::system_clock::now();
			Render(*meshGPU.resource, *materialGPU.resource, i_layout);
		}
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