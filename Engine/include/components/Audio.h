#pragma once

#include "Component.h"
#include <glm/vec3.hpp>
#include <fmod/fmod.hpp>

class AudioComponent : public Component
{
private:
	std::string m_name;
	FMOD::Sound* m_pSound;
	bool m_isStream;
	int m_idChannelGroup;
	float m_minDistance;
	float m_maxDistance;

	int m_currentChannelId;

public:
	virtual void GetInfo(sComponentInfo& compInfoOut);
	virtual void SetParameter(sParameterInfo& parameterIn);

	bool IsStream();
	std::string GetFileName();
	FMOD::Sound* GetSound();
	int GetChannelGroup();
	float GetMinDistance();
	float GetMaxDistance();
	int GetChannelId();

	void SetChannelId(int channelId);
	void SetSound(FMOD::Sound* pSound);
};