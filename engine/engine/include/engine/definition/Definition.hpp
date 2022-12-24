#pragma once
#if WITH_EDITOR
#include "editor/definition/DefinitionDef.hpp"
#endif

namespace engine
{
	template<typename Def, string_literal Name, auto... Props>
	struct definition
	{
#if WITH_EDITOR
		static editor::definition_def_impl<Def, Name, Props...> s_def;
		static const editor::definition_def& get_definition()
		{
			return s_def;
		}
#endif

		template<typename Func>
		static void visit(const Func& i_func)
		{
			Def::visit_fields(i_func);
		}

		template<typename Field, typename Func>
		static void visit_field(const Field& i_field, const Func& i_func)
		{
			i_func(i_field);
		}

		static void deserialize(Def& o_def)
		{
			auto func = [o_def](auto fieldPtr)
			{
				o_def.*fieldPtr;
			};
			visit(func);
		}


	};
#if WITH_EDITOR
	template<typename Def, string_literal Name, auto... Props>
	editor::definition_def_impl<Def, Name, Props...> definition<Def, Name, Props...>::s_def;
#endif
}

