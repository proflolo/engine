#pragma once

namespace engine
{
	class Module;

	class ModuleMap
	{
	public:
		ModuleMap(std::vector<std::unique_ptr<Module>> i_modules);

		template<typename TModule>
		requires std::is_base_of_v<Module, TModule>
		bool HasModule() const
		{
			auto it = std::find_if(m_modules.begin(), m_modules.end(), [](const auto& i_element) { return i_element->GetModuleId() == TModule::Id(); });
			return it != m_modules.end();
		}

		template<typename TModule>
		requires std::is_base_of_v<Module, TModule>
		TModule& GetModule() const
		{
			auto it = std::find_if(m_modules.begin(), m_modules.end(), [](const auto& i_element) { return i_element->GetModuleId() == TModule::Id(); });
			assert(it != m_modules.end());
			return static_cast<TModule&>(**it);
		}

		inline auto begin() const
		{
			return m_modules.begin();
		}

		inline auto end() const
		{
			return m_modules.end();
		}

	private:
		std::vector<std::unique_ptr<Module>> m_modules;
	};
}