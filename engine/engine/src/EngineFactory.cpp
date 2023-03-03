#include "StdAfx.h"
#include "EngineFactory.h"
#include "asset/AssetProvider.h"

namespace engine
{
	Context EngineFactory::CreateContext()
	{
		std::shared_ptr<AssetProvider> assetProvider = std::make_shared<AssetProvider>();
		return Context(std::move(assetProvider));
	}
}