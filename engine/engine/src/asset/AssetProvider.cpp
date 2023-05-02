#include "StdAfx.h"
#include "engine/asset/AssetProvider.h"

namespace engine
{
	struct AssetProvider::AssetContents
	{
		std::promise<std::vector<char>> promise;
	};

	void AssetProvider::LoadThread(std::stop_token i_stopToken)
	{
		while (!i_stopToken.stop_requested())
		{
			std::unique_lock dataLock(m_threadMutex);
			m_threadCondition.wait(dataLock, [this, i_stopToken] { return m_pendingLoads.size() > 0 || i_stopToken.stop_requested(); });
			if (i_stopToken.stop_requested())
			{
				return;
			}
			decltype(m_pendingLoads) processingLoads;
			processingLoads.swap(m_pendingLoads);
			dataLock.unlock();
			//Process all
			for (auto it = processingLoads.begin(); it != processingLoads.end() && !i_stopToken.stop_requested(); ++it)
			{
				if (i_stopToken.stop_requested())
				{
					return;
				}
				std::shared_ptr<engine::Asset<void>> asset;
				std::shared_ptr<AssetContents> contents;
				{
					std::scoped_lock lock(m_threadMutex);
					auto foundAsset = m_assetContents.find(*it);
					if (foundAsset != m_assetContents.end())
					{
						asset = foundAsset->first.lock();
						contents = foundAsset->second;
					}
					else
					{
						continue;
					}
				}
				std::ifstream stream(asset->GetPath().c_str(), std::ifstream::in);
				auto size = stream.tellg();
				std::vector<char> data(size);
				stream.seekg(0);
				stream.read(&data[0], size);
				stream.close();
				contents->promise.set_value(std::move(data));
			}
		}
	}
	
	AssetProvider::AssetProvider()
	{
		m_loadThread = std::jthread(&AssetProvider::LoadThread, this, m_readStop.get_token());
	}

	AssetProvider::~AssetProvider()
	{
		m_readStop.request_stop();
		m_threadCondition.notify_all();
		m_loadThread.join();
	}

	std::shared_future<std::vector<char>> AssetProvider::Load(const std::shared_ptr<Asset<void>>& i_asset)
	{
		//stop_callback
		auto it = m_assetContents.find(i_asset);
		if (it == m_assetContents.end())
		{
			std::unique_ptr<AssetContents> contents = std::make_unique<AssetContents>();
			std::shared_future<std::vector<char>> future = contents->promise.get_future();
			m_assetContents.emplace(i_asset, std::move(contents));
			{
				std::scoped_lock lock(m_threadMutex);
				m_pendingLoads.emplace_back(i_asset);
				m_threadCondition.notify_all();
			}
			return future;
		}
		else
		{
			std::shared_ptr<AssetContents> contents = it->second;
			return contents->promise.get_future();
		}
	}
}