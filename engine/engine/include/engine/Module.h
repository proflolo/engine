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
		virtual const engine::Db& GetDefinitions() const = 0;
		virtual RenderClient& GetRenderClient() = 0;
		virtual UpdateClient& GetUpdateClient() = 0;
		const Context& GetEngineContext() const;
	protected:
		Module(const Context& i_context);


	private:
		const Context m_context;

	};
}