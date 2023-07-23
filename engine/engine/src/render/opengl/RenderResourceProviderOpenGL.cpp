#include "StdAfx.h"
#if ENGINE_OPENGL
#include "RenderResourceProviderOpenGL.h"
#include "render/Mesh.h"
#include "render/Material.h"
#include "MeshOpenGL.h"
#include "MaterialOpenGL.h"
#include "OpenGLResult.h"
#include "asset/AssetProvider.h"

namespace engine
{
	
	RenderResourceProviderOpenGL::RenderResourceProviderOpenGL()
	{
		
	}

	RenderResourceProviderOpenGL::~RenderResourceProviderOpenGL()
	{
	}


	template<typename T, typename Params, typename Resource>
	std::shared_future<void> RenderResourceProviderOpenGL::_Load(const RenderResource<T>& io_resource, Params i_params, std::vector<ResourceHolder<Resource, Params>>& io_resources)
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
			return std::visit([index](auto& i_resource) -> std::shared_future<void>
				{
					using T = std::decay_t<decltype(i_resource)>;
					if constexpr (std::is_same_v<T, std::nullopt_t>)
					{
						assert(false);
						std::promise<void> incorrect;
						return incorrect.get_future();
					}
					else if constexpr (std::is_same_v<T, typename ResourceHolder<Resource, Params>::Resource>)
					{
						std::promise<void> completed;
						completed.set_value();
						return completed.get_future();
					}
					else if constexpr (std::is_same_v<T, ResourceHolder<Resource, Params>::Request>)
					{
						return i_resource.future;
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
			request.future = request.promise.get_future();
			std::shared_future<void> result = request.future;

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
	void RenderResourceProviderOpenGL::_ProcessLoads(std::vector<ResourceHolder<Resource, Params>>& io_resources, Args&... i_args)
	{
		using Holder = ResourceHolder<Resource, Params>;
		for (auto& itMaterial : io_resources)
		{
			auto& resource = itMaterial.resource;
			std::visit([&resource, &i_args...](auto& i_resource)
				{
					using T = std::decay_t<decltype(i_resource)>;
					using Req = ResourceHolder<Resource, Params>::Request;
					using Res = ResourceHolder<Resource, Params>::Resource;
					if constexpr (std::is_same_v<T, Req>)
					{
						i_resource.promise.set_value();
						resource.emplace<Res>(i_resource.source, Resource(i_resource.params, i_args...));
					}
				},
				itMaterial.resource);
		}
	}

	template<typename T, typename Params>
	inline void RenderResourceProviderOpenGL::ClearUnusedResources(std::vector<ResourceHolder<T, Params>>& io_resources)
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


	std::shared_future<void> RenderResourceProviderOpenGL::Load(const RenderResource<void>& i_resource)
	{
		if (i_resource.renderType == MaterialGeneric::k_renderType)
		{
			return Load(static_cast<const MaterialGeneric&>(i_resource));
		}
		else if (i_resource.renderType == MeshGeneric::k_renderType)
		{
			return Load(static_cast<const MeshGeneric&>(i_resource));
		}
		else
		{
			assert(false);
			return std::shared_future<void>();
		}
	}

	std::shared_future<void> RenderResourceProviderOpenGL::Load(const MaterialGeneric& i_material)
	{
		std::lock_guard guard(m_materialMutex);
		const std::optional<std::span<const char>> vcode = i_material.GetVertexShaderCode();
		const std::optional<std::span<const char>> fcode = i_material.GetVertexShaderCode();

		return _Load(i_material, GPUResourceMaterialOpenGLParams(*vcode, *fcode), m_materials);
	}

	std::shared_future<void> RenderResourceProviderOpenGL::Load(const MeshGeneric& i_mesh)
	{
		std::lock_guard guard(m_meshMutex);
		std::vector<float> data(i_mesh.GetData().begin(), i_mesh.GetData().end());
		return _Load(i_mesh, GPUResourceMeshOpenGLParams(std::move(data), i_mesh.GetIndices()), m_meshes);
	}

	void RenderResourceProviderOpenGL::ProcessLoads()
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

}
#endif