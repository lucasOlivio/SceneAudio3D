#pragma once

#include "DefaultObject.h"

class HangarDoorHandler : public DefaultObject
{
private:
	std::vector<std::string> m_doors;
	std::string m_audioDoor;

public:
	HangarDoorHandler() {};
	virtual ~HangarDoorHandler() {};

	virtual void OnStart(iEvent* pEvent);

	virtual void Interact(std::string tagThatInteracted, glm::vec3 position);
};
