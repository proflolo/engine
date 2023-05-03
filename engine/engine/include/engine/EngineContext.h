#pragma once

namespace engine
{
	class AssetProvider;
	class RenderResourceProvider;

	class Context
	{
	public:
		Context(std::shared_ptr<AssetProvider> i_assetProvider, RenderResourceProvider& i_renderResourceProvider);
		~Context();

		AssetProvider& GetAssetProvider() const;
		RenderResourceProvider& GetRenderResourceProvider() const;

	private:
		std::shared_ptr<AssetProvider> m_assetProvider;
		RenderResourceProvider& m_renderResourceProvider;
	};
}