#pragma once

namespace engine
{
	class UpdateClient
	{
	public:
		virtual void UpdatePrologue() {};
		virtual void Update() = 0;
		virtual void UpdateEpilogue() {};
	};
}