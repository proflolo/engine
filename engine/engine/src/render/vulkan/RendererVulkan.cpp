#include "StdAfx.h"
#if ENGINE_VULKAN
#include "RendererVulkan.h"
namespace engine
{
	RendererVulkan::RendererVulkan()
	{
		
	}

	RendererVulkan::~RendererVulkan()
	{
		Cleanup();
	}

	void RendererVulkan::Render(Mesh& i_mesh, Material& i_material)
	{

	}


	void RendererVulkan::Cleanup()
	{

	}
}

#endif