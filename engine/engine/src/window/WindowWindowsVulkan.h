#pragma once
#if PLATFORM_WINDOWS && !WITH_EDITOR && ENGINE_VULKAN
#include "WindowWindows.h"

namespace engine
{
	class RenderClient;

	class WindowWindowsVulkan: public WindowWindows
	{
	public:
		WindowWindowsVulkan(HINSTANCE hInstance, int nCmdShow, engine::RenderClient& i_renderClient);
		~WindowWindowsVulkan();

	private:
		
	};
}
#endif