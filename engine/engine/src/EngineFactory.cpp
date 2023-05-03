#include "StdAfx.h"
#include "EngineFactory.h"
#include "asset/AssetProvider.h"

namespace engine
{
	Context EngineFactory::CreateContext(RenderResourceProvider& i_renderResourceProvider)
	{
		std::shared_ptr<AssetProvider> assetProvider = std::make_shared<AssetProvider>();
		return Context(std::move(assetProvider), i_renderResourceProvider);
	}
}