#pragma once

namespace engine
{
	class definitions;

	class Module
	{
	public:
		virtual const engine::definitions& GetDefinitions() const = 0;
	};
}