#pragma once
#if ENGINE_OPENGL
#include "engine/render/Renderer.h"
#include "render/shadercompiler/SpirVCompiler.h"
namespace engine
{
	class GPUResourceOpenGL;
	class GPUResourceHolder;

	class RendererOpenGL : public Renderer
	{
	public:
		RendererOpenGL();
		~RendererOpenGL();

		void BeginRender(std::stop_token i_stopToken);
		void EndRender(std::stop_token i_stopToken);

		void Render(Mesh& i_mesh, Material& i_material) override;

		void FlagResourceForDeletion(std::unique_ptr<GPUResourceOpenGL>i_resource);

	private:

		static void DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

		std::vector<std::weak_ptr<GPUResourceHolder>> m_activeResources;
		std::vector<std::unique_ptr<GPUResourceOpenGL>> m_toDeleteResources;
		SpirVCompiler m_compiler;

		std::mutex m_deleteMutex;
	};
}
#endif