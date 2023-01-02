#pragma once

namespace engine
{
	class Renderer;

	class RenderClient
	{
	public:
		virtual ~RenderClient() = default;
		virtual void Render(std::stop_token i_stopToken, Renderer& i_renderer) = 0;
	};
}