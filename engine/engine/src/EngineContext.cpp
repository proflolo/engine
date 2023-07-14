#include "StdAfx.h"
#include "engine/EngineContext.h"

namespace engine
{
	Context::Context(std::shared_ptr<AssetProvider> i_assetProvider)
		: m_assetProvider(std::move(i_assetProvider))
	{

	}
	
	Context::~Context()
	{

	}
	AssetProvider& Context::GetAssetProvider() const
	{
		return *m_assetProvider;
	}

}