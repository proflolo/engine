#include "StdAfx.h"
#include "engine/EngineContext.h"

namespace engine
{
	Context::Context(std::shared_ptr<AssetProvider> i_assetProvider, RenderResourceProvider& i_renderResourceProvider)
		: m_assetProvider(std::move(i_assetProvider))
		, m_renderResourceProvider(i_renderResourceProvider)
	{

	}
	
	Context::~Context()
	{

	}
	AssetProvider& Context::GetAssetProvider() const
	{
		return *m_assetProvider;
	}
	
	RenderResourceProvider& Context::GetRenderResourceProvider() const
	{
		return m_renderResourceProvider;
	}
}