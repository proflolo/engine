#pragma once
#if PLATFORM_WINDOWS && !WITH_EDITOR && ENGINE_OPENGL
#include "WindowWindows.h"
#include "render/opengl/RenderResourceProviderOpenGL.h"
namespace engine
{
	class RenderClient;
	class Context;
	class RenderResourceProviderOpenGL;

	class WindowWindowsOpenGL: public WindowWindows
	{
	public:
		WindowWindowsOpenGL(HINSTANCE hInstance, int nCmdShow, engine::RenderClient& i_renderClient, engine::UpdateClient& i_updateClient);
		~WindowWindowsOpenGL();

	private:
		void Render(std::stop_token i_stopToken);
		std::jthread m_renderThread;
		std::stop_source m_stopSource;
		HDC m_hdc;
		HGLRC m_renderContext;
		PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
		RenderClient& m_renderClient;
		std::unique_ptr<RenderResourceProviderOpenGL> m_renderResourceProvider;
	};
}
#endif