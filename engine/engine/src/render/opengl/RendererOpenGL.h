#pragma once
#include "engine/render/Renderer.h"

namespace engine
{
	class RendererOpenGL : public Renderer
	{
	public:
		void BeginRender(std::stop_token i_stopToken);
		void EndRender(std::stop_token i_stopToken);
	};
}