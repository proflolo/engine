#pragma once

namespace engine
{
	class UpdateContext;

	class UpdateClient
	{
	public:
		virtual void UpdatePrologue() {};
		virtual void Update(const UpdateContext& i_updateContext) = 0;
		virtual void UpdateEpilogue() {};
	};
}