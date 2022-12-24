#pragma once
#include "engine/Module.h"

namespace render2D
{
	class Db;
	class Render2DModule : public engine::Module
	{
	public:
		Render2DModule();
		~Render2DModule();

		const engine::definitions& GetDefinitions() const override;

	private:
		std::unique_ptr<Db> m_db;
	};
}