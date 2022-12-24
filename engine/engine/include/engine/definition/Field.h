#pragma once
#if WITH_EDITOR
#include "editor/definition/FieldDef.h"
#endif

namespace engine
{
	

	template<typename T, string_literal Name, auto... Props>
	struct field
	{
	#if WITH_EDITOR
		static editor::field_def_impl<T, Name, Props...> s_def;
		static const editor::field_def& get_definition()
		{
			return s_def;
		}
	#endif
		T value;
	};

#if WITH_EDITOR
	template<typename T, string_literal Name, auto... Props>
	editor::field_def_impl<T, Name, Props...> field<T, Name, Props...>::s_def;
#endif
}

