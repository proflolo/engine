#pragma once
#include "engine/render/Renderer.h"

namespace engine
{
	class GPUResourceOpenGL;
	class GPUResourceHolder;

	class RendererOpenGL : public Renderer
	{
	public:
		RendererOpenGL();
		~RendererOpenGL();

		void BeginRender(std::stop_token i_stopToken);
		void EndRender(std::stop_token i_stopToken);

		void Render(Mesh& i_mesh) override;

		void FlagResourceForDeletion(std::unique_ptr<GPUResourceOpenGL>i_resource);

	private:
		std::vector<std::weak_ptr<GPUResourceHolder>> m_activeResources;
		std::vector<std::unique_ptr<GPUResourceOpenGL>> m_toDeleteResources;

		std::mutex m_deleteMutex;
	};
}