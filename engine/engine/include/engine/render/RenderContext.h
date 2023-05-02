#pragma once

namespace engine
{
	class Renderer;
	class RenderResourceProvider;

	class RenderContext
	{
	public:
		RenderContext(Renderer& i_renderer, RenderResourceProvider& i_resourceProvider);

		Renderer& GetRenderer() const;
		RenderResourceProvider& GetRenderResourceProvider() const;

	private:
		Renderer& m_renderer;
		RenderResourceProvider& m_renderResourceProvider;
	};
}