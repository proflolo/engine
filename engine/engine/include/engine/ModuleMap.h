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
			return m_modules.find(TModule::Id()) != m_modules.end();
		}

		template<typename TModule>
		requires std::is_base_of_v<Module, TModule>
		TModule& GetModule() const
		{
			return static_cast<TModule&>(*m_modules.at(TModule::Id()));
		}

	private:
		std::unordered_map<size_t, std::unique_ptr<Module>> m_modules;
	};
}