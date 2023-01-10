#include "StdAfx.h"
#if PLATFORM_WINDOWS && !WITH_EDITOR && ENGINE_VULKAN
#include "window/WindowWindowsVulkan.h"
#include "render/RenderClient.h"
//https://vulkan-tutorial.com/Drawing_a_triangle/Presentation/Window_surface


namespace engine
{
	WindowWindowsVulkan::WindowWindowsVulkan(HINSTANCE hInstance, int nCmdShow, engine::RenderClient& i_renderClient)
		: WindowWindows(hInstance, nCmdShow)
	{
		VkSurfaceKHR surface;
		VkWin32SurfaceCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.hwnd = m_hWnd;
		createInfo.hinstance = hInstance;
		
		if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}

	WindowWindowsVulkan::~WindowWindowsVulkan()
	{
		
	}	
}
#endif