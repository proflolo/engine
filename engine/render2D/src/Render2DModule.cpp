#include "StdAfx.h"
#include "Render2DModule.h"
#include "engine/definition/Definitionbase.h"

namespace render2D
{
	class Db: public engine::definitionbase<Db>
	{
	public:
		template<typename Func>
		static void visit_types(const Func& i_func)
		{
			
		}
	};

	Render2DModule::Render2DModule()
		: m_db(std::make_unique<Db>())
	{
	}

	Render2DModule::~Render2DModule()
	{
	}

	const engine::definitions& Render2DModule::GetDefinitions() const
	{
		return *m_db;
	}

}