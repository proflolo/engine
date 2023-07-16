#pragma once
#include "engine/update/UpdateClient.h"

namespace engine
{
	class UpdateClientCombiner : public UpdateClient
	{
	public:
		UpdateClientCombiner(std::vector<std::reference_wrapper<UpdateClient>> i_updateClients);

		void Update(const UpdateContext& i_uc) override;
		void UpdatePrologue() override;
		void UpdateEpilogue() override;
		

	private:
		std::vector<std::reference_wrapper<UpdateClient>> m_updateClients;


	};
}