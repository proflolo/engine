#include "StdAfx.h"
#include "UpdateClientCombiner.h"

namespace engine
{

	UpdateClientCombiner::UpdateClientCombiner(std::vector<std::reference_wrapper<UpdateClient>> i_updateClients)
		: m_updateClients(std::move(i_updateClients))
	{
	}

	void UpdateClientCombiner::Update(const UpdateContext& i_uc)
	{
		for (auto& updateClient : m_updateClients)
		{
			updateClient.get().Update(i_uc);
		}
	}

	void UpdateClientCombiner::UpdatePrologue()
	{
		for (auto& updateClient : m_updateClients)
		{
			updateClient.get().UpdatePrologue();
		}
	}

	void UpdateClientCombiner::UpdateEpilogue()
	{
		for (auto& updateClient : m_updateClients)
		{
			updateClient.get().UpdateEpilogue();
		}
	}

}