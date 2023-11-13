#include "GameObjects/Player.h"
#include "common/Input.h"
#include "common/utilsMat.h"
#include "common/opengl.h" // TODO: Remove this necessity, abstract key buttons values
#include <glm/gtc/matrix_transform.hpp>

bool Player::Load(EntityID entityID, SceneView* pScene)
{
	this->DefaultObject::Load(entityID, pScene);

	this->m_pCamera = pScene->GetComponent<CameraComponent>(entityID, "camera");

	return true;
}

void Player::OnStart(iEvent* pEvent)
{
	this->DefaultObject::OnStart(pEvent);

	this->m_currDirection = glm::vec3(0);
	this->m_firstUpdate = true;

	// Script custom
	//----------------
	this->m_sensitivity = std::stof(this->m_pScript->GetVariable("sensitivity"));

	pEvent->Attach(this);
}

void Player::Update(double deltaTime)
{
	this->Move(deltaTime);

	this->UpdateCamera();
}

void Player::Move(double deltaTime)
{
	using namespace glm;

	bool isMoving = false;

	vec3 newVelocity = vec3(0);
	vec3 cameraFront = this->m_pCamera->cameraFront;
	vec3 cameraUp = this->m_pCamera->upVector;
	vec3 direction = normalize(cross(cameraFront, cameraUp));

	// Front movement
	if (Input::IsKeyPressed(GLFW_KEY_W))
	{
		newVelocity += this->m_velocity * this->m_pCamera->cameraFront * (float)deltaTime;
		isMoving = true;
	}
	else if (Input::IsKeyPressed(GLFW_KEY_S))
	{
		newVelocity -= this->m_velocity * this->m_pCamera->cameraFront * (float)deltaTime;
		isMoving = true;
	}

	// Sides movement
	if (Input::IsKeyPressed(GLFW_KEY_D))
	{
		newVelocity += this->m_velocity * direction * (float)deltaTime;
		isMoving = true;
	}
	else if (Input::IsKeyPressed(GLFW_KEY_A))
	{
		newVelocity -= this->m_velocity * direction * (float)deltaTime;
		isMoving = true;
	}

	this->m_pForce->SetVelocity(newVelocity);

	if (isMoving)
	{
		this->m_pCamera->cameraEye = this->m_pTransform->GetPosition();
	}
}

void Player::UpdateCamera()
{
	using namespace glm;

	vec2 mousePos = Input::MousePosition();
	float xpos = mousePos.x;
	float ypos = mousePos.y;

	if (this->m_firstUpdate)
	{
		this->m_lastX = xpos;
		this->m_lastY = ypos;
		this->m_firstUpdate = false;
	}

	float xoffset = xpos - this->m_lastX;
	float yoffset = this->m_lastY - ypos; // reversed since y-coordinates go from bottom to top
	this->m_lastX = xpos;
	this->m_lastY = ypos;

	xoffset *= m_sensitivity;
	yoffset *= m_sensitivity;

	this->m_yaw += xoffset;
	this->m_pitch += yoffset;

	// make sure that when this->m_pitch is out of bounds, screen doesn't get flipped
	if (this->m_pitch > 89.0f)
		this->m_pitch = 89.0f;
	if (this->m_pitch < -89.0f)
		this->m_pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(this->m_yaw)) * cos(glm::radians(this->m_pitch));
	front.y = sin(glm::radians(this->m_pitch));
	front.z = sin(glm::radians(this->m_yaw)) * cos(glm::radians(this->m_pitch));
	this->m_pCamera->cameraFront = glm::normalize(front);
}
