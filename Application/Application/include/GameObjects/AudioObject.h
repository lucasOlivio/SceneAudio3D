#pragma once

#include "DefaultObject.h"
#include "components/Audio.h"

class AudioObject : public DefaultObject
{
private:
	AudioComponent* m_pAudio;

	// Script custom
	//----------------
	bool m_destroyAfter;
	bool m_isPlaying;
	bool m_isPausable;
	float m_pitch;

	bool m_setReverb;
	float m_reverbDecay;
	float m_reverbDensity;
	float m_reverbDiffusion;

	bool m_setDistortion;
	float m_distortionLevel;

	bool m_setHighPass;
	bool m_setLowPass;

public:
	AudioObject() {};
	virtual ~AudioObject() {};

	virtual bool Load(EntityID entityID, SceneView* pScene);

	void OnStart(iEvent* pEvent);

	virtual void Update(double deltaTime);

	virtual void OnExit(iEvent* pEvent);

	virtual void Interact(std::string tagThatInteracted, glm::vec3 position);
};
