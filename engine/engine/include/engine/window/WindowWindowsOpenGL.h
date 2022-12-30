#pragma once
#include "WindowWindows.h"

namespace engine
{
	class WindowWindowsOpenGL: public WindowWindows
	{
	public:
		WindowWindowsOpenGL(HINSTANCE hInstance, int nCmdShow);
		~WindowWindowsOpenGL();

	private:
		void Render(std::stop_token i_stopToken);
		std::jthread m_renderThread;
		std::stop_source m_stopSource;
		HDC m_hdc;
		HGLRC m_renderContext;
	};
}