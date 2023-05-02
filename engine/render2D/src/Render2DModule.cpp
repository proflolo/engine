#include "StdAfx.h"
#include "Render2DModule.h"
#include "engine/definition/Definitionbase.hpp"

namespace render2D
{
	class Db: public engine::definitionbase<Db>
	{
	public:
		template<typename Func>
		static void visit_types(const Func& i_func)
		{
			
		}

		template<typename Func>
		static void visit_assets(const Func& i_func)
		{

		}
	};

	Render2DModule::Render2DModule(const engine::Context& i_context)
		: engine::Module(i_context)
		, m_db(std::make_unique<Db>())
	{
	}

	Render2DModule::~Render2DModule()
	{
	}

	const engine::Db& Render2DModule::GetDefinitions() const
	{
		return *m_db;
	}

	engine::RenderClient& Render2DModule::GetRenderClient()
	{
		return *this;
	}

	engine::UpdateClient& Render2DModule::GetUpdateClient()
	{
		return *this;
	}

	void Render2DModule::Update()
	{

	}

	void Render2DModule::Render(std::stop_token i_stopToken, const engine::RenderContext& i_renderer)
	{

	}

}