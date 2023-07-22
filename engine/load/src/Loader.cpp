#include "StdAfx.h"
#include "Loader.h"
#include "engine/asset/AssetProvider.h"
#include "engine/render/RenderContext.h"
#include "engine/render/RenderResourceProvider.h"

namespace load
{
	void Loader::Enqueue(std::span <std::shared_ptr<engine::Asset<void>>> i_assets, std::span<engine::RenderResource<void>> i_renderResource)
	{
		m_tasks.emplace_back(LoadTask{ i_assets, i_renderResource });
	}

	void Loader::Enqueue(engine::MaterialGeneric& i_material)
	{
		std::span<engine::RenderResource<void>> renders(static_cast<engine::RenderResource<void>*>(& i_material), 1);
		Enqueue(i_material.GetAssets(), renders);
	}

	void Loader::Enqueue(engine::MeshGeneric& i_mesh)
	{
		std::span<engine::RenderResource<void>> renders(static_cast<engine::RenderResource<void>*>(&i_mesh), 1);
		Enqueue({}, renders);
	}

	namespace
	{
		template<typename T>
		bool has_finished(const std::shared_future<T>& i_future)
		{
			return i_future.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready;
		}


	}
	void Loader::Render(std::stop_token i_stopToken, const engine::RenderContext& i_renderer)
	{
		decltype(m_renderCalls) currentList;
		currentList.swap(m_renderCalls);
		for (auto func : currentList)
		{
			func(i_renderer);
		}
	}

	void Loader::LoadThread(std::unique_ptr<Loader, LoadModule::LoadDeleter> i_loader, std::stop_token i_stopToken, const engine::Context& i_engineContext)
	{
		std::unordered_map<size_t, std::vector<std::shared_future<void>>> assetFutures;
		//Enqueue all assets
		{
			size_t index = 0;
			for (LoadTask& task : i_loader->m_tasks)
			{
				std::vector<std::shared_future<void>>& futures = assetFutures[index];
				for (const std::shared_ptr<engine::Asset<void>>& asset : task.assets)
				{
					if (i_stopToken.stop_requested()) return;
					futures.emplace_back(i_engineContext.GetAssetProvider().Load(asset));
				}
				index++;
			}
		}

		//Now, keep iterating to see progress. If ALL assets are loaded, then load graphic resources
		{
			std::unordered_map<size_t, std::vector<std::shared_future<void>>> renderFutures;
			bool allTasksFinished = false;
			while (!allTasksFinished && !i_stopToken.stop_requested())
			{
				allTasksFinished = true;
				size_t index = 0;
				for (LoadTask& task : i_loader->m_tasks)
				{
					std::vector<std::shared_future<void>>& diskFutures = assetFutures[index];
					bool allDiskFinished = true;
					for (const std::shared_future<void>& future : diskFutures)
					{
						if (i_stopToken.stop_requested()) return;
						allDiskFinished = allDiskFinished && has_finished(future);
					}

					if (allDiskFinished)
					{
						if (i_stopToken.stop_requested()) return;
						std::vector<std::shared_future<void>>& futures = renderFutures[index];
						if (task.renderResource.size() > 0 && futures.size() == 0)
						{
							futures.resize(task.renderResource.size());
							if (i_stopToken.stop_requested()) return;
							size_t renderIdx = 0;
							for (engine::RenderResource<void>& renderResource : task.renderResource)
							{
								if (i_stopToken.stop_requested()) return;
								std::shared_future<void> renderLoadFuture;
								i_loader->m_renderCalls.emplace_back([&renderLoadFuture, renderResource](const engine::RenderContext& i_context)
									{
										renderLoadFuture = i_context.GetRenderResourceProvider().Load(renderResource);
									});
#error
								futures[renderIdx] = renderLoadFuture;
								renderIdx++;
							}
						}

						for (std::shared_future<void>& renderFuture : futures)
						{
							if (!has_finished(renderFuture))
							{
								allTasksFinished = false;
							}
						}
					}
					else
					{
						allTasksFinished = false;
					}
					index++;
				}
			}
		}
	}

	std::future<void> Loader::Run(std::unique_ptr<Loader, LoadModule::LoadDeleter> i_loader, std::stop_token i_stopToken, const engine::Context& i_engineContext)
	{
		return std::async(std::launch::async, [i_stopToken, i_engineContext, loader = std::move(i_loader)]() mutable { LoadThread(std::move(loader), i_stopToken, i_engineContext); });
	}
}