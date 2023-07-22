#include "StdAfx.h"
#include "LoadModule.h"
#include "engine/definition/Definitionbase.hpp"

namespace load
{
	

	LoadModule::LoadModule(const engine::Context& i_context)
		: engine::ModuleImplementation<LoadModule>(i_context)
	{
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

	}

}