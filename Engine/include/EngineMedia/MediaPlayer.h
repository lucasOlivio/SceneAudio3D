#pragma once

#include "scene/SceneView.h"
#include "AudioManager.h"
#include <glm/mat4x4.hpp>

class MediaPlayer
{
private:
	AudioManager* m_pAudioManager;
	SceneView* m_pSceneView;

	//Singleton
	MediaPlayer();

	static MediaPlayer* m_pInstance;
public:
	~MediaPlayer();

	static MediaPlayer* Get();

	// Where we really start/destroy everything that could go wrong
	bool Initialize(std::string baseAudiosPath, SceneView* pSceneView);
	bool LoadChannels();
	bool LoadAudios();
	void Destroy();

	void Update(double deltaTime);

	void SetListenerAttributes(const glm::vec3& position, const glm::vec3& velocity,
								const glm::vec3& forward, const glm::vec3& up);

	void PlayAudio(EntityID entityId, glm::vec3 soundVelocity);
	void StopAudio(EntityID entityId);
};