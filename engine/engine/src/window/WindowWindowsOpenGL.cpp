#include "StdAfx.h"
#if PLATFORM_WINDOWS && !WITH_EDITOR && ENGINE_OPENGL
#include "window/WindowWindowsOpenGL.h"
#include "render/opengl/RendererOpenGL.h"
#include "render/RenderClient.h"
#include "render/RenderContext.h"
#include "render/opengl/RenderResourceProviderOpenGL.h"

//#include "platform/opengl/glxext.h"
//https://registry.khronos.org/OpenGL/index_gl.php

//https://mariuszbartosik.com/opengl-4-x-initialization-in-windows-without-a-framework/

LRESULT FakeWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, uMsg, wParam, lParam);

}

namespace engine
{
	WindowWindowsOpenGL::WindowWindowsOpenGL(HINSTANCE hInstance, int nCmdShow, engine::RenderClient& i_renderClient, engine::UpdateClient& i_updateClient)
		: WindowWindows(hInstance, nCmdShow, i_updateClient)
		, m_renderClient(i_renderClient)
		, m_renderResourceProvider(std::make_unique<RenderResourceProviderOpenGL>())
	{
		m_hdc = GetDC(m_hWnd);
		if (!m_hdc)
		{
			auto err = GetLastError();
			throw std::runtime_error("Could not get DC");
		}
		
		
		std::wstring name = L"Core";
		std::wstring title = L"Fake";
		
		HICON hIcon = NULL;
		WCHAR szExePath[MAX_PATH];
		GetModuleFileName(NULL, szExePath, MAX_PATH);
		
		hIcon = ExtractIcon(m_hInstance, szExePath, 0);
		
		
		WNDCLASS wndClass;
		wndClass.style = CS_DBLCLKS;
		wndClass.lpfnWndProc = FakeWindowProc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hInstance = m_hInstance;
		wndClass.hIcon = hIcon;
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = name.c_str();
		
		bool success = RegisterClass(&wndClass);
		if (!success)
		{
			auto err = GetLastError();
			throw std::runtime_error("Could not register fake window");
		}

		HWND fakeWND = CreateWindow(
			name.c_str(), title.c_str(),      // window class, title
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN, // style
			0, 0,					    // position x, y
			1, 1,					    // width, height
			NULL, NULL,				    // parent window, menu
			m_hInstance, NULL);		    // instance, param

		if (fakeWND == nullptr)
		{
			auto err = GetLastError();
			throw std::runtime_error("Could not create fake Window");
		}

		HDC fakeDC = GetDC(fakeWND);        // Device Context

		PIXELFORMATDESCRIPTOR fakePFD;
		ZeroMemory(&fakePFD, sizeof(fakePFD));
		fakePFD.nSize = sizeof(fakePFD);
		fakePFD.nVersion = 1;
		fakePFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		fakePFD.iPixelType = PFD_TYPE_RGBA;
		fakePFD.cColorBits = 32;
		fakePFD.cAlphaBits = 8;
		fakePFD.cDepthBits = 24;

		int fakePFDID = ChoosePixelFormat(fakeDC, &fakePFD);
		if (fakePFDID == 0) {
			throw std::runtime_error("ChoosePixelFormat failed");
		}

		if (SetPixelFormat(fakeDC, fakePFDID, &fakePFD) == false) {
			throw std::runtime_error("SetPixelFormat failed");
		}

		HGLRC fakeRC = wglCreateContext(fakeDC);	// Rendering Contex

		if (fakeRC == 0) {
			throw std::runtime_error("error creating fake DC");
		}

		if (wglMakeCurrent(fakeDC, fakeRC) == false) {
			throw std::runtime_error("error making current gl context");
		}

		
		wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));
		if (wglChoosePixelFormatARB == nullptr) {
			throw std::runtime_error("wglChoosePixelFormatARB not found");
		}

		wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
		if (wglCreateContextAttribsARB == nullptr) {
			throw std::runtime_error("wglCreateContextAttribsARB not found");
		}

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(fakeRC);
		ReleaseDC(fakeWND, fakeDC);
		DestroyWindow(fakeWND);

		
		m_renderThread = std::jthread(&WindowWindowsOpenGL::Render, this, m_stopSource.get_token());
		
		
	}

	WindowWindowsOpenGL::~WindowWindowsOpenGL()
	{
		m_stopSource.request_stop();
		m_renderThread.join();
		wglDeleteContext(m_renderContext);
		ReleaseDC(m_hWnd, m_hdc);
	}

	void WindowWindowsOpenGL::Render(std::stop_token i_stopToken)
	{
		const int pixelAttribs[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_ALPHA_BITS_ARB, 8,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
			WGL_SAMPLES_ARB, 4,
			0
		};

		int pixelFormatID; 
		UINT numFormats;
		bool status = wglChoosePixelFormatARB(m_hdc, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats);

		if (status == false || numFormats == 0) {
			throw std::runtime_error("failed wglChoosePixelFormatARB");
		}

		PIXELFORMATDESCRIPTOR PFD;
		DescribePixelFormat(m_hdc, pixelFormatID, sizeof(PFD), &PFD);
		SetPixelFormat(m_hdc, pixelFormatID, &PFD);

		const int major_min = 4, minor_min = 6;
		int  contextAttribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, major_min,
			WGL_CONTEXT_MINOR_VERSION_ARB, minor_min,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};

		m_renderContext = wglCreateContextAttribsARB(m_hdc, 0, contextAttribs);
		if (m_renderContext == NULL) {
			throw std::runtime_error("Could not create render context");
		}

		if (!wglMakeCurrent(m_hdc, m_renderContext)) {
			throw std::runtime_error("Could not make current context");
		}

		PFNWGLSWAPINTERVALEXTPROC       wglSwapIntervalEXT = NULL;
		PFNWGLGETSWAPINTERVALEXTPROC    wglGetSwapIntervalEXT = NULL;

		//Vsync
		// Extension is supported, init pointers.
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

		// this is another function from WGL_EXT_swap_control extension
		wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
		
		wglSwapIntervalEXT(1);

		auto err = glewInit();
		if (err != GLEW_OK)
		{
			throw std::runtime_error("wglCreateContextAttribsARB not found");
		}

		{
			RendererOpenGL renderer(*m_renderResourceProvider);
			RenderContext context(renderer, *m_renderResourceProvider);
			renderer.BeginRendering();
			while (!i_stopToken.stop_requested())
			{
				renderer.BeginFrame(i_stopToken);
				m_renderClient.Render(i_stopToken, context);
				renderer.EndFrame(i_stopToken);
				m_renderResourceProvider->ProcessLoads();
				SwapBuffers(m_hdc);
			}
			renderer.EndRendering();
		}
		m_renderResourceProvider.reset();
	}	
}
#endif