#pragma once
#include "Properties.h"
#include "PropertyElement.h"

namespace editor
{
	class field_def
	{
	public:
		virtual editor::order GetOrder() const = 0;
		virtual std::string GetName() const = 0;
		virtual std::string GetUiName() const = 0;
		//virtual Type GetType() const = 0;
	};

	template<typename T, engine::string_literal Name, auto... Props>
	requires (... && FieldProperty<decltype(Props), T>)
	class field_def_impl : public field_def, public property_element<Props...>
	{
	public:
		using props = property_element<Props...>;

		editor::order GetOrder() const override
		{
			return props:: template get_property<editor::order>();
		}

		std::string GetName() const override
		{
			return Name;
		}

		std::string GetUiName() const override
		{
			return props:: template get_property<ui_name>().value;
		}

		//Type GetType() const override
		//{
		//	return type_def<T>::value;
		//}
	};
}