#include "GameObjects/GameManager.h"
#include "GameObjects/GameObjectFactory.h"
#include "common/utils.h"
#include "common/utilsMat.h"
#include "components/Transform.h"

GameManager::~GameManager()
{
}

void GameManager::GameOver()
{
}

bool GameManager::Load(EntityID entityID, SceneView* pScene)
{
	this->m_pSceneView = pScene;
	this->m_entityID = entityID;

	this->m_pTag = pScene->GetComponent<TagComponent>(entityID, "tag");
	this->m_pScript = pScene->GetComponent<ScriptComponent>(entityID, "script");

	return true;
}

void GameManager::OnStart(iEvent* pEvent)
{
	using namespace std;
	using namespace myutils;
	using namespace glm;

	this->m_pCollisionEvents = pEvent;

	// Script custom
	//----------------
	this->m_maxHeight = std::stof(this->m_pScript->GetVariable("maxHeight"));
	this->m_maxWidth = std::stof(this->m_pScript->GetVariable("maxWidth"));
	this->m_minHeight = std::stof(this->m_pScript->GetVariable("minHeight"));
	this->m_minWidth = std::stof(this->m_pScript->GetVariable("minWidth"));

	GameObjectFactory gameObjFactory(this->m_pSceneView);

	// Load player
	//----------------
	iGameObject* pPlayer = gameObjFactory.CreateGameObject("player", false);

	pPlayer->SetMediator(this);
	pPlayer->OnStart(this->m_pCollisionEvents);

	this->m_pPlayer = pPlayer;
}

void GameManager::Update(double deltaTime)
{
	this->m_pPlayer->Update(deltaTime);
}

void GameManager::OnExit(iEvent* pEvent)
{
	for (iGameObject* pGameObj : this->m_vecGameObjects)
	{
		pGameObj->OnExit(pEvent);
		delete pGameObj;
	}
}

std::string GameManager::GetTagName()
{
	return this->m_pTag->name;
}

std::string GameManager::GetScriptName()
{
	return this->m_pScript->GetScriptName();
}

EntityID GameManager::GetEntityID()
{
	return this->m_entityID;
}

bool GameManager::GetTargetDirection(std::string targetName, glm::vec3 objPosition, glm::vec3& directionOut)
{
	for (iGameObject* pGameObj : this->m_vecGameObjects)
	{
		if (pGameObj->GetScriptName() == targetName)
		{
			glm::vec3 targetPosition = pGameObj->GetPosition();
			directionOut = myutils::GetCardinalDirection(targetPosition, objPosition);
			return true;
		}
	}

	return false;
}

void GameManager::DestroyGameObj(iGameObject* pGameObj)
{
	using namespace std;

	string tagName = pGameObj->GetTagName();

	if (tagName == "player")
	{
		this->GameOver();
	}

	pGameObj->Delete();
	// Clean from scene
	this->m_pSceneView->DeleteEntity(pGameObj->GetEntityID());
	this->m_pSceneView->DeleteListener(pGameObj);
}

void GameManager::SpawnGameObj(std::string tagName, glm::vec3 position, glm::vec3 direction)
{
	using namespace glm;
	using namespace myutils;

	GameObjectFactory gameObjFactory(this->m_pSceneView);

	iGameObject* pGameObj = gameObjFactory.CreateGameObject(tagName, true);

	pGameObj->SetMediator(this);
	pGameObj->OnStart(this->m_pCollisionEvents);

	pGameObj->SetPosition(position);
	pGameObj->SetDirection(direction);

	this->m_vecGameObjects.push_back(pGameObj);
}

void GameManager::SpawnGameObj(std::string tagName)
{
	using namespace glm;
	using namespace myutils;

	// Avoid enemy been placed at player position
	float playerCenterMin = 450;
	float playerCenterMax = 550;

	vec3 position(0);
	position.x = GetRandFloat((float)this->m_minWidth, (float)this->m_maxWidth, playerCenterMin, playerCenterMax);
	position.y = GetRandFloat((float)this->m_minHeight, (float)this->m_maxHeight, playerCenterMin, playerCenterMax);

	this->SpawnGameObj(tagName, position, vec3(0));
}
