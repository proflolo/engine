#pragma once
#if ENGINE_OPENGL
#include "engine/render/Renderer.h"
#include "engine/render/RenderResource.h"
#include "render/shadercompiler/SpirVCompiler.h"
#include "engine/render/RenderResourceProvider.h"

namespace engine
{
	class GPUResourceOpenGL;
	class MeshOpenGL;
	class MaterialOpenGL;
	class GPUResourceMeshOpenGL;
	class GPUResourceMaterialOpenGL;
	struct RenderLayout;
	struct GPUResourceMaterialOpenGLParams;
	struct GPUResourceMeshOpenGLParams;
	class RenderResourceProviderOpenGL;

	class RendererOpenGL : public Renderer
	{
	public:
		RendererOpenGL(RenderResourceProviderOpenGL& i_provider);
		~RendererOpenGL();

		void BeginRendering();
		void EndRendering();

		void BeginFrame(std::stop_token i_stopToken);
		void EndFrame(std::stop_token i_stopToken);

		void Render(MeshGeneric& i_mesh, MaterialGeneric& i_material, const RenderLayout& i_layout) override;

		
	private:
		
		void Render(GPUResourceMeshOpenGL& i_mesh, GPUResourceMaterialOpenGL& i_material, const RenderLayout& i_layout);
		void BindLayout(GLuint i_arrayIndex, const RenderLayout& i_layout);
		static void DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

		RenderResourceProviderOpenGL& m_resourceProvider;
	};



}
#endif