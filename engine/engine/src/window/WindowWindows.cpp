#include "StdAfx.h"
#if PLATFORM_WINDOWS && !WITH_EDITOR
#include "window/WindowWindows.h"
#include "update/UpdateClient.h"
#include "update/UpdateContext.h"
namespace engine
{
	WindowWindows::WindowWindows(HINSTANCE hInstance, int nCmdShow, UpdateClient& i_updateClient)
		: m_hInstance(hInstance)
		, m_windowClassName(L"lolo")
		, m_updateClient(i_updateClient)
	{
		HICON hIcon = NULL;
		WCHAR szExePath[MAX_PATH];
		GetModuleFileName(NULL, szExePath, MAX_PATH);

		if (hIcon == NULL)
		{
			hIcon = ExtractIcon(m_hInstance, szExePath, 0);
		}

		// Register the windows class
		WNDCLASS wndClass;
		wndClass.style = CS_DBLCLKS;
		wndClass.lpfnWndProc = WindowWindows::StaticWindowProc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hInstance = m_hInstance;
		wndClass.hIcon = hIcon;
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = m_windowClassName.c_str();

		bool success = RegisterClass(&wndClass);

		if (!success)
		{
			DWORD dwError = GetLastError();
			if (dwError != ERROR_CLASS_ALREADY_EXISTS)
			{
				//return HRESULT_FROM_WIN32(dwError);
			}
		}

		int x = 0; // CW_USEDEFAULT;
		int y = 0; // CW_USEDEFAULT;


		// This example uses a non-resizable 640 by 480 viewport for simplicity.
		int nDefaultWidth = 640;
		int nDefaultHeight = 480;
		//SetRect(m_rc, 0, 0, nDefaultWidth, nDefaultHeight);
		//AdjustWindowRect(m_rc, WS_OVERLAPPEDWINDOW, false);
		// Create the window for our viewport.
		m_hWnd = CreateWindow(
			m_windowClassName.c_str(),
			L"Cube11",
			//WS_OVERLAPPEDWINDOW,
			WS_CAPTION | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			x, y,
			nDefaultWidth, nDefaultHeight,
			0,
			NULL,
			m_hInstance,
			0
		);

		if (m_hWnd == NULL)
		{
			DWORD dwError = GetLastError();
			//return HRESULT_FROM_WIN32(dwError);
		}

		ShowWindow(m_hWnd, nCmdShow);
		UpdateWindow(m_hWnd);

		m_updateThread = std::jthread(&WindowWindows::Update, this, m_updateStopSource.get_token());

	}

	WindowWindows::~WindowWindows()
	{
		m_updateStopSource.request_stop();
		m_updateThread.join();
		UnregisterClass(m_windowClassName.c_str(), m_hInstance);
		DestroyWindow(m_hWnd);
	}

	int WindowWindows::Run()
	{
		bool bGotMsg;
		MSG  msg;
		msg.message = WM_NULL;
		PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

		while (WM_QUIT != msg.message)
		{
			// Process window events.
			// Use PeekMessage() so we can use idle time to render the scene. 
			bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);

			if (bGotMsg)
			{
				// Translate and dispatch the message
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				// Update the scene.
				//renderer->Update();

				// Render frames during idle time (when no messages are waiting).
				//renderer->Render();

				// Present the frame to the screen.
				//deviceResources->Present();
			}
		}
		return 0;
	}

	LRESULT WindowWindows::StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CLOSE:
		{
			HMENU hMenu;
			hMenu = GetMenu(hWnd);
			if (hMenu != NULL)
			{
				DestroyMenu(hMenu);
			}
			DestroyWindow(hWnd);
			return 0;
		}

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	
	void WindowWindows::Update(std::stop_token i_stopToken)
	{
		while (!i_stopToken.stop_requested())
		{
			UpdateContext context;
			m_updateClient.UpdatePrologue();
			m_updateClient.Update(context);
			m_updateClient.UpdateEpilogue();
		}
	}
}
#endif