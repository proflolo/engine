#pragma once
#include "Window.h"

namespace engine
{
	class WindowWindows : public Window
	{
	public:
		WindowWindows(HINSTANCE hInstance, int nCmdShow);
		~WindowWindows();
		int Run();
	private:
		static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		HINSTANCE m_hInstance;
		HWND m_hWnd;
		LPRECT m_rc;
		std::wstring m_windowClassName;
	};
}