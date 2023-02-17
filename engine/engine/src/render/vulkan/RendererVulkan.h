#pragma once
#if ENGINE_VULKAN
#include "render/Renderer.h"

namespace engine
{
	class RendererVulkan: public Renderer
	{
	public:
		RendererVulkan();
		~RendererVulkan();

		void Render(MeshGeneric& i_mesh, Material& i_material) override;

	private:
		
		void Cleanup();
	};
}
#endif