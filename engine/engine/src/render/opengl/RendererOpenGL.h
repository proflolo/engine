#pragma once
#if ENGINE_OPENGL
#include "engine/render/Renderer.h"
#include "render/RenderResource.h"
#include "render/shadercompiler/SpirVCompiler.h"
namespace engine
{
	class GPUResourceOpenGL;
	class MeshOpenGL;
	class MaterialOpenGL;
	class GPUResourceMeshOpenGL;
	class GPUResourceMaterialOpenGL;
	struct RenderLayout;

	class RendererOpenGL : public Renderer
	{
	public:
		RendererOpenGL();
		~RendererOpenGL();

		void BeginRender(std::stop_token i_stopToken);
		void EndRender(std::stop_token i_stopToken);

		void Render(MeshGeneric& i_mesh, MaterialGeneric& i_material, const RenderLayout& i_layout) override;

		
	private:


		void Render(GPUResourceMeshOpenGL& i_mesh, GPUResourceMaterialOpenGL& i_material, const RenderLayout& i_layout);
		void BindLayout(GLuint i_arrayIndex, const RenderLayout& i_layout);
		static void DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
		template<typename T>
		struct ResourceHolder
		{
			ResourceHolder(T&& i_resource, std::chrono::system_clock::time_point i_lastUsed)
				: resource(std::move(i_resource))
				, lastUsed(std::move(i_lastUsed))
			{

			}

			ResourceHolder(const ResourceHolder<T>&) = delete;
			ResourceHolder(ResourceHolder<T>&& i_other)
				: resource(std::move(i_other.resource))
				, lastUsed(std::move(i_other.lastUsed))
			{

			}
			std::chrono::system_clock::time_point lastUsed;
			std::optional<T> resource;
		};
		std::vector<ResourceHolder<GPUResourceMeshOpenGL>> m_meshes;
		std::vector<ResourceHolder<GPUResourceMaterialOpenGL>> m_materials;

		template<typename T>
		void ClearUnusedResources(std::vector<ResourceHolder<T>>& io_resources);

		template<typename R, typename T>
		RenderResource<R>::Id AllocateResource(std::vector<ResourceHolder<T>>& io_resources, T&& i_resource);
		
		SpirVCompiler m_compiler;

		std::mutex m_deleteMutex;
	};



}
#endif