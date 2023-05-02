#include "StdAfx.h"
#include "engine/render/RenderContext.h"

namespace engine
{
	RenderContext::RenderContext(Renderer& i_renderer, RenderResourceProvider& i_resourceProvider)
		: m_renderer(i_renderer)
		, m_renderResourceProvider(i_resourceProvider)
	{

	}
	Renderer& RenderContext::GetRenderer() const
	{
		return m_renderer;
	}
	RenderResourceProvider& RenderContext::GetRenderResourceProvider() const
	{
		return m_renderResourceProvider;
	}
}