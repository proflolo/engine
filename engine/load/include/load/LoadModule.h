#pragma once
#include "engine/Module.h"
#include "engine/render/RenderClient.h"
#include "engine/update/UpdateClient.h"
#include "engine/definition/Definitionbase.hpp"

namespace load
{
	class Loader;
	class Db;
	class LoadModule : public engine::ModuleImplementation<LoadModule>, public engine::RenderClient, public engine::UpdateClient
	{
	public:
		LoadModule(const engine::Context& i_context);
		~LoadModule();

		const engine::Db& GetDefinitions() const override;

		engine::RenderClient& GetRenderClient() override;
		engine::UpdateClient& GetUpdateClient() override;

		void Update(const engine::UpdateContext& i_uc) override;
		void Render(std::stop_token i_stopToken, const engine::RenderContext& i_renderer) override;

		struct LoadDeleter
		{
			void operator()(Loader* i_loader) const;
			LoadModule* module;
		};
		std::unique_ptr<Loader, LoadDeleter> CreateLoader();

	private:
		engine::Db m_db;
		LoadDeleter m_loadDeleter;
		std::unordered_set<Loader*> m_loaders;
	};
}