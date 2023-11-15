#include "GameObjects/HangarDoorHandler.h"

void HangarDoorHandler::OnStart(iEvent* pEvent)
{
	this->DefaultObject::OnStart(pEvent);

	// Script custom
	//----------------
	myutils::SplitString(this->m_pScript->GetVariable("doors"), ' ', this->m_doors);
	this->m_audioDoor = this->m_pScript->GetVariable("audioDoor");
}

void HangarDoorHandler::Interact(std::string tagThatInteracted, glm::vec3 position)
{
	iGameMediator* pGameMediator = this->GetMediator();

	for (std::string doorTag : this->m_doors)
	{
		pGameMediator->InteractWith(doorTag, this->GetTagName(), this->GetPosition());
	}

	pGameMediator->InteractWith(this->m_audioDoor, this->GetTagName(), this->GetPosition());
}
