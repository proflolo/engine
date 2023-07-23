#pragma once
#include "LoadModule.h"

namespace engine
{
	class RenderContext;
}

namespace load
{
	class Loader
	{
	public:
		
		void Enqueue(std::vector <std::shared_ptr<engine::Asset<void>>> i_assets, std::vector<engine::RenderResource<void>*> i_renderResource);
		
		//Helper functions with some engine classes
		void Enqueue(engine::MaterialGeneric& i_material);
		void Enqueue(engine::MeshGeneric& i_mesh);

		static std::future<void> Run(std::unique_ptr<Loader, LoadModule::LoadDeleter> i_loader, std::stop_token i_stopToken, const engine::Context& i_engineContext);

	private:
		Loader() = default;
		void Render(std::stop_token i_stopToken, const engine::RenderContext& i_renderer);

		static void LoadThread(std::unique_ptr<Loader, LoadModule::LoadDeleter> i_loader, std::stop_token i_stopToken, const engine::Context& i_engineContext);

		struct LoadTask
		{
			std::vector <std::shared_ptr<engine::Asset<void>>> assets;
			std::vector<engine::RenderResource<void>*> renderResource;
		};

		std::vector<LoadTask> m_tasks;
		std::vector<std::function<void(const engine::RenderContext& i_context)>> m_renderCalls;

		friend class LoadModule;
	};
}