#include "StdAfx.h"
#if ENGINE_OPENGL
#include "RendererOpenGL.h"
#include "render/Mesh.h"
#include "render/Material.h"
#include "MeshOpenGL.h"
#include "MaterialOpenGL.h"
#include "OpenGLResult.h"
#include "asset/AssetProvider.h"
#include "RenderResourceProviderOpenGL.h"

namespace engine
{
	
	RendererOpenGL::RendererOpenGL(RenderResourceProviderOpenGL& i_resourceProvider)
		: m_resourceProvider(i_resourceProvider)
	{
		
	}

	RendererOpenGL::~RendererOpenGL()
	{
	}

	void RendererOpenGL::BeginRendering()
	{
		glEnable(GL_DEBUG_OUTPUT);
#if !WITH_EDITOR
		glDebugMessageCallback(DebugMessageCallback, 0);
#endif
	}

	void RendererOpenGL::EndRendering()
	{
		
	}

	void RendererOpenGL::BeginFrame(std::stop_token i_stopToken)
	{
		glClearColor(0.129f, 0.586f, 0.949f, 1.0f);	// rgb(33,150,243)
		glClear(GL_COLOR_BUFFER_BIT);
	}



	

	void RendererOpenGL::EndFrame(std::stop_token i_stopToken)
	{
		
		
	}

	void RendererOpenGL::Render(MeshGeneric& i_mesh, MaterialGeneric& i_material, const RenderLayout& i_layout)
	{
		std::lock_guard guardMaterials(m_resourceProvider.m_materialMutex);
		std::lock_guard guardMeshes(m_resourceProvider.m_meshMutex);

		if (!i_mesh.HasRenderResource())
		{
			return;
		}

		if (!i_material.HasRenderResource())
		{
			return;
		}


		auto& meshGPU = m_resourceProvider.m_meshes[i_mesh.renderResource];
		auto& materialGPU = m_resourceProvider.m_materials[i_material.renderResource];

		if (!meshGPU.IsReady())
		{
			return;
		}

		if (!materialGPU.IsReady())
		{
			return;
		}

		
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