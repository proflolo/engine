#pragma once
#include "FieldDef.hpp"

namespace editor
{
	namespace detail
	{
		template <class C, typename T>
		T get_pointer_type(T C::* v);
	}

	class definition_def
	{
	public:
		virtual std::vector<std::reference_wrapper<const field_def>> list_fields() const = 0;
		virtual std::string GetName() const = 0;
		virtual std::string GetUiName() const = 0;
	};

	template<typename Def, engine::string_literal Name, DefinitionProperty auto... Props>
	class definition_def_impl : virtual public definition_def, public property_element<Props...>
	{
	public:
		using props = property_element<Props...>;
		std::vector<std::reference_wrapper<const editor::field_def>> list_fields() const override
		{
			std::vector<std::reference_wrapper<const editor::field_def>> result;
			auto func = [&result](auto fieldPtr)
			{
				using T = decltype(detail::get_pointer_type(fieldPtr));
				result.emplace_back(T::get_definition());
			};
			Def::visit(func);
			return result;
		}

		std::string GetName() const override
		{
			return Name;
		}

		std::string GetUiName() const override
		{
			return props:: template get_property<ui_name>().value;
		}
	};

	class asset_def: virtual public definition_def
	{
	public:
		virtual std::string GetExtension() const = 0;
	};

	template<typename Def, engine::string_literal Name, engine::string_literal Extension, DefinitionProperty auto... Props>
	class asset_def_impl : public definition_def_impl<Def, Name, Props...>, virtual public asset_def
	{
	public:
		
		std::string GetExtension() const override
		{
			return Extension;
		}
	};
}