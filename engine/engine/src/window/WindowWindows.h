#pragma once
#if PLATFORM_WINDOWS && !WITH_EDITOR
#include "window/Window.h"

namespace engine
{
	class UpdateClient;

	class WindowWindows : public Window
	{
	public:
		WindowWindows(HINSTANCE hInstance, int nCmdShow, UpdateClient& i_updateClient);
		~WindowWindows();
		int Run();
	private:
		static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		LPRECT m_rc;
		std::wstring m_windowClassName;

	protected:
		HINSTANCE m_hInstance;
		HWND m_hWnd;

	private:
		void Update(std::stop_token i_stopToken);

		UpdateClient& m_updateClient;
		std::jthread m_updateThread;
		std::stop_source m_updateStopSource;
	};
}
#endif