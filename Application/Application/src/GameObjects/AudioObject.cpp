#include "GameObjects/AudioObject.h"
#include "EngineMedia/MediaPlayer.h"

bool AudioObject::Load(EntityID entityID, SceneView* pScene)
{
	this->DefaultObject::Load(entityID, pScene);

	this->m_pAudio = pScene->GetComponent<AudioComponent>(entityID, "audio");

	return true;
}

void AudioObject::OnStart(iEvent* pEvent)
{
	this->DefaultObject::OnStart(pEvent);

	// Script custom
	//----------------
	this->m_startPlaying = std::stoi(this->m_pScript->GetVariable("startPlaying"));

	MediaPlayer::Get()->PlayAudio(this->GetEntityID(), glm::vec3(0, 0, 0));
}

void AudioObject::Update(double deltaTime)
{
}

void AudioObject::OnExit(iEvent* pEvent)
{
	this->DefaultObject::OnExit(pEvent);

	MediaPlayer::Get()->StopAudio(this->GetEntityID());
}
