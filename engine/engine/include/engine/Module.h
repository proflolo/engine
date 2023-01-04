#pragma once

namespace engine
{
	class definitions;

	class Module
	{
	public:
		virtual ~Module() = default;
		virtual const engine::definitions& GetDefinitions() const = 0;
	};
}