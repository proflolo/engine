#include "StdAfx.h"
#include <Windows.h>
#include "window/WindowWindowsOpenGL.h"
#include <gl/GL.h>

#include "platform/opengl/glext.h"
#include "platform/opengl/glcorearb.h"
#include "platform/opengl/wglext.h"
//#include "platform/opengl/glxext.h"
//https://registry.khronos.org/OpenGL/index_gl.php

//https://mariuszbartosik.com/opengl-4-x-initialization-in-windows-without-a-framework/
namespace engine
{
	WindowWindowsOpenGL::WindowWindowsOpenGL(HINSTANCE hInstance, int nCmdShow)
		: WindowWindows(hInstance, nCmdShow)
	{
		m_hdc = GetDC(m_hWnd);
		if (!m_hdc)
		{

		}
		
		//static	PIXELFORMATDESCRIPTOR pfd =					// pfd Tells Windows How We Want Things To Be
		//{
		//	sizeof(PIXELFORMATDESCRIPTOR),					// Size Of This Pixel Format Descriptor
		//	1,								// Version Number
		//	PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		//	PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		//	PFD_DOUBLEBUFFER,						// Must Support Double Buffering
		//	PFD_TYPE_RGBA,							// Request An RGBA Format
		//	32,								// Select Our Color Depth
		//	0, 0, 0, 0, 0, 0,						// Color Bits Ignored
		//	0,								// No Alpha Buffer
		//	0,								// Shift Bit Ignored
		//	0,								// No Accumulation Buffer
		//	0, 0, 0, 0,							// Accumulation Bits Ignored
		//	16,								// 16Bit Z-Buffer (Depth Buffer)
		//	0,								// No Stencil Buffer
		//	0,								// No Auxiliary Buffer
		//	PFD_MAIN_PLANE,							// Main Drawing Layer
		//	0,								// Reserved
		//	0, 0, 0								// Layer Masks Ignored
		//};
		//
		//pfd.cAlphaBits = 8;
		//
		//auto format_index = ::ChoosePixelFormat(hdc, &pfd);
		//if (!format_index)
		//{
		//
		//}
		//
		//bool success = SetPixelFormat(hdc, format_index, &pfd);
		//if (!success)
		//{
		//
		//}
		//
		//auto active_format_index = GetPixelFormat(hdc);
		//if (!active_format_index)
		//{
		//	
		//}
		//
		//if (!DescribePixelFormat(hdc, active_format_index, sizeof pfd, &pfd))
		//{
		//	
		//}
		//
		//if ((pfd.dwFlags & PFD_SUPPORT_OPENGL) != PFD_SUPPORT_OPENGL)
		//{
		//	
		//}

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
		HWND fakeWND = CreateWindow(
			L"Core", L"Fake Window",      // window class, title
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN, // style
			0, 0,					    // position x, y
			1, 1,					    // width, height
			NULL, NULL,				    // parent window, menu
			m_hInstance, NULL);		    // instance, param

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
			
		}

		if (SetPixelFormat(fakeDC, fakePFDID, &fakePFD) == false) {
			
		}

		HGLRC fakeRC = wglCreateContext(fakeDC);	// Rendering Contex

		if (fakeRC == 0) {
			
		}

		if (wglMakeCurrent(fakeDC, fakeRC) == false) {
			
		}


		PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
		wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));
		if (wglChoosePixelFormatARB == nullptr) {

		}

		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
		wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
		if (wglCreateContextAttribsARB == nullptr) {

		}

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
			
		}

		PIXELFORMATDESCRIPTOR PFD;
		DescribePixelFormat(m_hdc, pixelFormatID, sizeof(PFD), &PFD);
		SetPixelFormat(m_hdc, pixelFormatID, &PFD);

		const int major_min = 4, minor_min = 5;
		int  contextAttribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, major_min,
			WGL_CONTEXT_MINOR_VERSION_ARB, minor_min,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};

		m_renderContext = wglCreateContextAttribsARB(m_hdc, 0, contextAttribs);
		if (m_renderContext == NULL) {
			
		}

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(fakeRC);
		ReleaseDC(fakeWND, fakeDC);
		DestroyWindow(fakeWND);
		

		if (!wglMakeCurrent(m_hdc, m_renderContext)) {
			
		}

		PFNWGLSWAPINTERVALEXTPROC       wglSwapIntervalEXT = NULL;
		PFNWGLGETSWAPINTERVALEXTPROC    wglGetSwapIntervalEXT = NULL;

		//Vsync
		// Extension is supported, init pointers.
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

		// this is another function from WGL_EXT_swap_control extension
		wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
		
		wglSwapIntervalEXT(1);

		while (!i_stopToken.stop_requested())
		{
			glClearColor(0.129f, 0.586f, 0.949f, 1.0f);	// rgb(33,150,243)
			glClear(GL_COLOR_BUFFER_BIT);
			SwapBuffers(m_hdc);
		}

		int i = 0;
		++i;
		//int const create_attribs[] = {
		//	WGL_CONTEXT_MAJOR_VERSION_ARB, ctx->m_major_version,
		//	WGL_CONTEXT_MINOR_VERSION_ARB, ctx->m_minor_version,
		//	0
		//};
		//HGLRC glrc = wglCreateContext(m_hdc);
		//HGLRC glrc2 = wglCreateContextAttribsARB(m_hdc, 0, create_attribs);
		//
		//if (glrc2) {
		//	if (!glrc2.make_current(dc))
		//		return EXIT_FAILURE;
		//	advrc.swap(glrc);
		//}
	}

	
}