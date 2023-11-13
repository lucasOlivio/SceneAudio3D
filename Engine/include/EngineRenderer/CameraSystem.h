#pragma once

#include "scene/SceneView.h"
#include "components/Camera.h"
#include "components/Transform.h"
#include <glm/mat4x4.hpp>

class CameraSystem
{
private:
	SceneView* m_pSceneView;

	CameraComponent* m_pCamera;
public:
	// ctors & dtors
	CameraSystem(SceneView* pSceneView);
	~CameraSystem();

	void Initialize();

	glm::mat4 GetViewMat();
};