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

	class RenderResourceProviderOpenGL : public RenderResourceProvider
	{
	public:
		RenderResourceProviderOpenGL();
		~RenderResourceProviderOpenGL();

		std::shared_future<void> Load(const RenderResource<void>& i_material) override;
		std::shared_future<void> Load(const MaterialGeneric& i_material) override;
		std::shared_future<void> Load(const MeshGeneric& i_mesh) override;

		void ProcessLoads();

		
	private:
		
		
		

		template<typename T, typename Params>
		struct ResourceHolder
		{
			struct Request
			{
				Params params;
				std::promise<void> promise;
				std::shared_future<void> future;
				const void* source;
				size_t index;
			};

			struct Resource
			{
				Resource(const void* i_source, T i_value)
					: source(i_source)
					, value(std::move(i_value))
				{

				}
				const void* source;
				T value;
			};

			ResourceHolder(Resource&& i_resource, std::chrono::system_clock::time_point i_lastUsed);



			ResourceHolder(Request&& i_request, std::chrono::system_clock::time_point i_lastUsed);

			ResourceHolder(const ResourceHolder<T, Params>&) = delete;
			ResourceHolder(ResourceHolder<T, Params>&& i_other);

			inline bool IsReady() const
			{
				return std::visit([](const auto& i_resource)->bool
					{
						using T = std::decay_t<decltype(i_resource)>;
						return std::is_same_v<T, Resource>;
					}, resource);
			}

			inline bool IsEmpty() const
			{
				return std::visit([](const auto& i_resource)->bool
					{
						using T = std::decay_t<decltype(i_resource)>;
						return std::is_same_v<T, std::nullopt_t>;
					}, resource);
			}

			inline T& GetResource()
			{
				return std::visit([](auto& i_resource) -> T&
					{
						using R = std::decay_t<decltype(i_resource)>;
						if constexpr (std::is_same_v<R, Resource>)
						{
							return i_resource.value;
						}
						else
						{
							assert(false);
							static T* s_dummy;
							return *s_dummy;
						}
					}
				
				, resource);
			}

			std::chrono::system_clock::time_point lastUsed;
			std::variant<std::nullopt_t, Resource, Request> resource;
		};

		template<typename T, typename Params, typename Resource>
		inline static std::shared_future<void> _Load(const RenderResource<T>& i_material, Params i_params, std::vector<ResourceHolder<Resource, Params>>& io_resources);

		template<typename Params, typename Resource, typename... Args>
		inline static void _ProcessLoads(std::vector<ResourceHolder<Resource, Params>>& io_resources, Args&... i_args);

		std::vector<ResourceHolder<GPUResourceMeshOpenGL, GPUResourceMeshOpenGLParams>> m_meshes;
		std::vector<ResourceHolder<GPUResourceMaterialOpenGL, GPUResourceMaterialOpenGLParams>> m_materials;

		template<typename T, typename Params>
		void ClearUnusedResources(std::vector<ResourceHolder<T, Params>>& io_resources);

		SpirVCompiler m_compiler;

		//todo: remove those mutexes. Just enqueue requests and in the processloads function, lock and swap.
		std::mutex m_deleteMutex;
		std::mutex m_materialMutex;
		std::mutex m_meshMutex;

		friend class RendererOpenGL;
	};



	template<typename T, typename Params>
	inline RenderResourceProviderOpenGL::ResourceHolder<T, Params>::ResourceHolder(Resource&& i_resource, std::chrono::system_clock::time_point i_lastUsed)
		: resource(std::move(i_resource))
		, lastUsed(std::move(i_lastUsed))
	{

	}

	template<typename T, typename Params>
	inline RenderResourceProviderOpenGL::ResourceHolder<T, Params>::ResourceHolder(Request&& i_request, std::chrono::system_clock::time_point i_lastUsed)
		: resource(std::move(i_request))
		, lastUsed(std::move(i_lastUsed))
	{

	}

	template<typename T, typename Params>
	inline RenderResourceProviderOpenGL::ResourceHolder<T, Params>::ResourceHolder(ResourceHolder<T, Params>&& i_other)
		: resource(std::move(i_other.resource))
		, lastUsed(std::move(i_other.lastUsed))
	{

	}

}
#endif