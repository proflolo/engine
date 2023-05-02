#pragma once
#include "engine/Module.h"
#include "engine/render/RenderClient.h"
#include "engine/update/UpdateClient.h"

namespace render2D
{
	class Db;
	class Render2DModule : public engine::Module, public engine::RenderClient, public engine::UpdateClient
	{
	public:
		Render2DModule(const engine::Context& i_context);
		~Render2DModule();

		const engine::Db& GetDefinitions() const override;

		engine::RenderClient& GetRenderClient() override;
		engine::UpdateClient& GetUpdateClient() override;

		void Update() override;
		void Render(std::stop_token i_stopToken, const engine::RenderContext& i_renderer) override;

	private:
		std::unique_ptr<Db> m_db;

	};
}