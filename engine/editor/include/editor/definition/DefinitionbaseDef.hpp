#pragma once
#include "DefinitionDef.hpp"

namespace editor
{
	class definitionbase_def
	{
	public:
		definitionbase_def(const definitionbase_def&) = delete;
		definitionbase_def(definitionbase_def&&) = delete;
		virtual std::vector<std::reference_wrapper<const editor::definition_def>> list_defs() const = 0;
	protected:
		definitionbase_def() = default;
	};

	template<typename Def>
	class definitionbase_def_impl : public definitionbase_def
	{
		std::vector<std::reference_wrapper<const editor::definition_def>> list_defs() const override
		{
			std::vector<std::reference_wrapper<const editor::definition_def>> result;
			auto func = [&result](auto* i_nullSeed) //Do not use seed
			{
				using Def = std::decay_t<decltype(*i_nullSeed)>;
				result.emplace_back(std::ref(Def::get_definition()));
			};
			Def::visit_types(func);
			return result;
		}

	};
}