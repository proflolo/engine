#include "StdAfx.h"
#include "LoadModule.h"
#include "engine/definition/Definitionbase.hpp"
#include "Loader.h"

namespace load
{
	

	LoadModule::LoadModule(const engine::Context& i_context)
		: engine::ModuleImplementation<LoadModule>(i_context)
	{
		m_loadDeleter.module = this;
	}

	LoadModule::~LoadModule()
	{
	}

	const engine::Db& LoadModule::GetDefinitions() const
	{
		return m_db;
	}

	engine::RenderClient& LoadModule::GetRenderClient()
	{
		return *this;
	}

	engine::UpdateClient& LoadModule::GetUpdateClient()
	{
		return *this;
	}

	void LoadModule::Update(const engine::UpdateContext& i_uc)
	{

	}

	void LoadModule::Render(std::stop_token i_stopToken, const engine::RenderContext& i_renderer)
	{
		for (Loader* loader : m_loaders)
		{
			loader->Render(i_stopToken, i_renderer);
		}
	}

	std::unique_ptr<Loader, LoadModule::LoadDeleter> LoadModule::CreateLoader()
	{
		std::unique_ptr<Loader, LoadModule::LoadDeleter> loader = std::unique_ptr<Loader, LoadModule::LoadDeleter>(new Loader(), m_loadDeleter);
		m_loaders.emplace(loader.get());
		return loader;
	}

	void LoadModule::LoadDeleter::operator()(Loader* i_loader) const
	{
		module->m_loaders.erase(i_loader);
		delete i_loader;
	}

}