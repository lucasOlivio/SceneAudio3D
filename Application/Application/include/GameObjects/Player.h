#pragma once

#include "DefaultObject.h"
#include "components/Camera.h"

class Player : public DefaultObject
{
private:
	// First person movement
	CameraComponent* m_pCamera;

	// Flying camera
	bool m_firstUpdate;
	float m_lastX;
	float m_lastY;
	float m_yaw;
	float m_pitch;
	float m_sensitivity;

public:
	Player() {};
	virtual ~Player() {};

	virtual bool Load(EntityID entityID, SceneView* pScene);

	void OnStart(iEvent* pEvent);

	virtual void Update(double deltaTime);

	// Set direction to move
	void Move(double deltaTime);

	void UpdateCamera();
};
