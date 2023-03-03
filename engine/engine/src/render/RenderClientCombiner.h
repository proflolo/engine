#pragma once
#include "engine/render/RenderClient.h"

namespace engine
{
	class RenderClientCombiner : public RenderClient
	{
	public:
		RenderClientCombiner(std::vector<std::reference_wrapper<RenderClient>> i_renderClients);

		void Render(std::stop_token i_stopToken, Renderer& i_renderer) override;

	private:
		std::vector<std::reference_wrapper<RenderClient>> m_renderClients;

	};
}