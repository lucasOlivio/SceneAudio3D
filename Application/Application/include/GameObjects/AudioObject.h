#pragma once

#include "DefaultObject.h"
#include "components/Audio.h"

class AudioObject : public DefaultObject
{
private:
	AudioComponent* m_pAudio;

	// Script custom
	//----------------
	bool m_startPlaying;

public:
	AudioObject() {};
	virtual ~AudioObject() {};

	virtual bool Load(EntityID entityID, SceneView* pScene);

	void OnStart(iEvent* pEvent);

	virtual void Update(double deltaTime);

	virtual void OnExit(iEvent* pEvent);
};
