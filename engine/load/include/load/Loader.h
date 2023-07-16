#pragma once

namespace engine
{
	class RenderContext;
}

namespace load
{
	class Loader
	{
	public:
		void Enqueue(std::span <std::shared_ptr<engine::Asset<void>>> i_assets, std::span<engine::RenderResource<void>> i_renderResource);
		
		//Helper functions with some engine classes
		void Enqueue(engine::MaterialGeneric& i_material);
		void Enqueue(engine::MeshGeneric& i_mesh);

		static std::future<void> Run(Loader&& i_loader, std::stop_token i_stopToken, const engine::Context& i_engineContext);

	private:

		static void LoadThread(Loader i_loader, std::stop_token i_stopToken, const engine::Context& i_engineContext);

		struct LoadTask
		{
			std::span <std::shared_ptr<engine::Asset<void>>> assets;
			std::span<engine::RenderResource<void>> renderResource;
		};

		std::vector<LoadTask> m_tasks;
		std::vector<std::function<void(const engine::RenderContext& i_context)>> m_renderCalls;

	};
}