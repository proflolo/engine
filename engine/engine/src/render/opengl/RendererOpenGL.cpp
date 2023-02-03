#include "StdAfx.h"
#if ENGINE_OPENGL
#include "RendererOpenGL.h"
#include "render/Mesh.h"
#include "render/Material.h"
#include "GPUResourceMeshOpenGL.h"
#include "GPUResourceMaterialOpenGL.h"
#include "OpenGLResult.h"

namespace engine
{
	class GPUResourceHolder: public GPUResource
	{
	public:
		GPUResourceHolder(RendererOpenGL& i_renderer, std::unique_ptr<GPUResourceOpenGL> i_realResource)
			: renderer(&i_renderer)
			, realResource(std::move(i_realResource))
		{

		}

		~GPUResourceHolder()
		{
			if (renderer)
			{
				renderer->FlagResourceForDeletion(std::move(realResource));
				renderer = nullptr;
			}
		}

		std::unique_ptr<GPUResourceOpenGL> Extract()
		{
			renderer = nullptr;
			return std::move(realResource);
		}

		RendererOpenGL* renderer;
		std::unique_ptr<GPUResourceOpenGL> realResource;
	};

	RendererOpenGL::RendererOpenGL()
	{
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(DebugMessageCallback, 0);
	}

	RendererOpenGL::~RendererOpenGL()
	{
		for (const std::weak_ptr<GPUResourceHolder>& activeResource : m_activeResources)
		{
			if (std::shared_ptr<GPUResourceHolder> resource = activeResource.lock())
			{
				resource->Extract();
			}
		}
	}

	void RendererOpenGL::BeginRender(std::stop_token i_stopToken)
	{
		glClearColor(0.129f, 0.586f, 0.949f, 1.0f);	// rgb(33,150,243)
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void RendererOpenGL::EndRender(std::stop_token i_stopToken)
	{
		decltype(m_toDeleteResources) toDelete;
		{
			std::scoped_lock lock(m_deleteMutex);
			std::swap(toDelete, m_toDeleteResources);
		}
	}

	template<typename T>
	inline T& GetGPUResource(Resource& i_resource)
	{
		return static_cast<T&>(*i_resource.GetGPUResource<GPUResourceHolder>().realResource);
	}

	void RendererOpenGL::Render(Mesh& i_mesh, Material& i_material)
	{
		if (!i_mesh.HasGPUResource())
		{
			//Create it!
			std::unique_ptr<GPUResourceMeshOpenGL> gpuResource = std::make_unique<GPUResourceMeshOpenGL>();
			std::shared_ptr<GPUResourceHolder> handle = std::make_shared<GPUResourceHolder>(*this, std::move(gpuResource));
			m_activeResources.emplace_back(handle);
			i_mesh.AssignGPUResource(std::move(handle));
		}

		if (!i_material.HasGPUResource())
		{
			//Create it!
			std::unique_ptr<GPUResourceMaterialOpenGL> gpuResource = std::make_unique<GPUResourceMaterialOpenGL>(m_compiler);
			std::shared_ptr<GPUResourceHolder> handle = std::make_shared<GPUResourceHolder>(*this, std::move(gpuResource));
			m_activeResources.emplace_back(handle);
			i_material.AssignGPUResource(std::move(handle));
		}

		GPUResourceMeshOpenGL& meshGL = GetGPUResource<GPUResourceMeshOpenGL>(i_mesh);
		GPUResourceMaterialOpenGL& materialGL = GetGPUResource<GPUResourceMaterialOpenGL>(i_material);
		
		materialGL.Use();
		
		glBindVertexArray(meshGL.GetVertexArray());
		glBindBuffer(GL_ARRAY_BUFFER, meshGL.GetVertexBuffer());
		opengl_check();

		glEnableVertexAttribArray(0);
		opengl_check();
		glVertexAttribPointer(
			0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,	// size
			GL_FLOAT, // type
			GL_FALSE, // normalized?
			0, // stride
			(void*)0 // array buffer offset
		);
		opengl_check();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshGL.GetIndexBuffer());
		opengl_check();

		// Draw the triangles !
		glDrawElements(
			GL_TRIANGLES,      // mode
			meshGL.GetIndexSize(),    // count
			GL_UNSIGNED_INT,   // type
			(void*)0           // element array buffer offset
		);

		opengl_check();

	}

	void RendererOpenGL::FlagResourceForDeletion(std::unique_ptr<GPUResourceOpenGL> i_resource)
	{
		std::scoped_lock lock(m_deleteMutex);
		m_toDeleteResources.emplace_back(std::move(i_resource));
	}

	void RendererOpenGL::DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{

	}
}
#endif