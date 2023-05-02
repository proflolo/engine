#include "StdAfx.h"
#include "RenderClientCombiner.h"

namespace engine
{
	RenderClientCombiner::RenderClientCombiner(std::vector<std::reference_wrapper<RenderClient>> i_renderClients)
		: m_renderClients(std::move(i_renderClients))
	{
	}
	
	void RenderClientCombiner::Render(std::stop_token i_stopToken, const RenderContext& i_renderer)
	{
		for (auto& renderClient : m_renderClients)
		{
			renderClient.get().Render(i_stopToken, i_renderer);
		}
	}
}
