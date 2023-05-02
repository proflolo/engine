#pragma once

namespace engine
{
	class RenderContext;

	class RenderClient
	{
	public:
		virtual ~RenderClient() = default;
		virtual void Render(std::stop_token i_stopToken, const RenderContext& i_renderer) = 0;
	};
}