#pragma once

namespace engine
{
	class AssetProvider;

	class Context
	{
	public:
		Context(std::shared_ptr<AssetProvider> i_assetProvider);
		~Context();

		AssetProvider& GetAssetProvider() const;

	private:
		std::shared_ptr<AssetProvider> m_assetProvider;
	};
}