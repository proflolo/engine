#pragma once

namespace engine
{
	class RenderResourceProvider;
	class EngineFactory
	{
	public:
		static Context CreateContext(RenderResourceProvider& i_renderResourceProvider);
	};
}