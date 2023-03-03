#pragma once


namespace engine
{
	class AssetProvider
	{
	public:
		AssetProvider();
		std::shared_future<std::vector<char>> ReadAsset(const std::shared_ptr<Asset<void>>& i_asset);

	private:

		struct AssetContents;

		void LoadThread(std::stop_token i_stopToken);

		std::map<std::weak_ptr<Asset<void>>, std::shared_ptr<AssetContents>, std::owner_less<std::weak_ptr<Asset<void>>>> m_assetContents;
		std::mutex m_threadMutex;
		std::condition_variable m_threadCondition;
		std::vector<std::weak_ptr<Asset<void>>> m_pendingLoads;
		std::jthread m_loadThread;
		std::stop_source m_readStop;
	};
}