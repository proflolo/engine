#pragma once
#include "render/Renderer.h"

namespace engine
{
	class RendererVulkan: public Renderer
	{
	public:
		RendererVulkan();
		~RendererVulkan();

		void Render(Mesh& i_mesh, Material& i_material) override;

	private:
		
		void Cleanup();
	};
}