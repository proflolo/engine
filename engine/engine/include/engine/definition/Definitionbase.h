#pragma once
#if WITH_EDITOR
#include "editor/definition/DefinitionbaseDef.h"
#endif

namespace engine
{
	class definitions
	{
	public:
#if WITH_EDITOR
		virtual const editor::definitionbase_def& get_definition() = 0;
#endif
	};

	template<typename Db>
	class definitionbase: public definitions
	{
	public:
#if WITH_EDITOR
		static editor::definitionbase_def_impl<Db> s_def;

		const editor::definitionbase_def& get_definition() override
		{
			return s_def;
		}
#endif
		template<typename Def, typename Func>
		static void visit_type(const Func& i_func)
		{
			Def* dummy = nullptr;
			i_func(dummy);
		}


	private:

	};
#if WITH_EDITOR
	template<typename Db>
	editor::definitionbase_def_impl<Db> definitionbase<Db>::s_def;
#endif
}