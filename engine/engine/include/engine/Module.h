#pragma once


namespace engine
{
	class Db;
	class RenderClient;
	class UpdateClient;

	class Module
	{
	public:
		virtual ~Module() = default;
		Module(const Module&) = delete;
		virtual const engine::Db& GetDefinitions() const = 0;
		virtual RenderClient& GetRenderClient() = 0;
		virtual UpdateClient& GetUpdateClient() = 0;
		inline const Context& GetEngineContext() const { return m_context; }
		virtual const size_t GetModuleId() const = 0;
	
	private:
		inline Module(const Context& i_context) : m_context(i_context) {};
		const Context& m_context;


		template<typename Base>
		friend class ModuleImplementation;

	};
	
	template<typename Base>
	class ModuleImplementation: public Module
	{
	protected:
		ModuleImplementation(const Context& i_context) : Module(i_context) {}
		
	private:
		static uint32_t k_id;
	public:
		const size_t GetModuleId() const override
		{
			return (size_t)&k_id;
		}

		inline static const size_t Id()
		{
			return (size_t)&k_id;
		}
	};

	template<typename Base>
	uint32_t ModuleImplementation<Base>::k_id = 0;

	
	

}