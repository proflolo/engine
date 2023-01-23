#pragma once
#if PLATFORM_WINDOWS && !WITH_EDITOR && ENGINE_VULKAN
#include "WindowWindows.h"

namespace engine
{
	class RenderClient;
	class RendererVulkan;

	class WindowWindowsVulkan: public WindowWindows
	{
	public:
		WindowWindowsVulkan(HINSTANCE hInstance, int nCmdShow, engine::RenderClient& i_renderClient);
		~WindowWindowsVulkan();

	private:
		void CreateInstance(const std::vector<const char*>& i_validationLayers);
		bool CheckValidationLayerSupport(const std::vector<const char*>& i_validationLayers) const;
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		std::vector<const char*> GetRequiredExtensions() const;
		void SetupDebugMessenger();
		void CreateSurface(HINSTANCE hInstance);
		void PickPhysicalDevice();
		void CreateLogicalDevice(const std::vector<const char*>& i_validationLayers);

		void Cleanup();
		VkInstance m_instance;
		VkSurfaceKHR m_surface;
		VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
		VkDevice m_device;
		VkQueue m_graphicsQueue;
		VkQueue m_presentQueue;

		VkDebugUtilsMessengerEXT m_debugMessenger;
		std::unique_ptr<RendererVulkan> m_renderer;

		bool m_enableValidationLayers = true;
	};
}
#endif