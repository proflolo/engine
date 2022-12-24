#pragma once

namespace editor
{
	struct property
	{

	};

	template<typename FieldT>
	struct field_property
	{

	};

	struct definition_property
	{

	};

	template<typename T, typename FieldT>
	concept FieldProperty = std::is_base_of_v<field_property<void>, T> || std::is_base_of_v<field_property<FieldT>, T>;

	template<typename T>
	concept DefinitionProperty = std::is_base_of_v<definition_property, T>;

	struct order : public property, public field_property<void>
	{
		constexpr order()
			: value(0)
		{

		}
		constexpr order(int i_value)
			: value(i_value)
		{

		}
		const int value;
	};


	struct ui_name : public property, public field_property<void>, public definition_property
	{
		constexpr ui_name() {}
		constexpr ui_name(engine::string_literal str)
			: value(str)
		{}

		engine::string_literal value;
	};

	struct ui_range : public property, public field_property<int>, public definition_property
	{
		constexpr ui_range() : min(0), max(0) {}
		constexpr ui_range(int i_min, int i_max)
			: min(i_min)
			, max(i_max)
		{

		}

		const int min;
		const int max;
	};
}