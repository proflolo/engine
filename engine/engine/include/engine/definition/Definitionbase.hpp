#pragma once
#if WITH_EDITOR
#include "editor/definition/DefinitionbaseDef.hpp"
#endif

namespace engine
{
	class Db
	{
	public:
#if WITH_EDITOR
		virtual const editor::definitionbase_def& get_definition() const = 0;
#endif
	};

	template<typename Database>
	class definitionbase: public Db
	{
	public:
#if WITH_EDITOR
		static editor::definitionbase_def_impl<Database> s_def;

		const editor::definitionbase_def& get_definition() const override
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