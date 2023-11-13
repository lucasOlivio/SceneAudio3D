#include "GameObjects/GameObjectFactory.h"
#include "GameObjects/GameManager.h"

GameObjectFactory::GameObjectFactory(SceneView* pScene)
{
	this->m_pScene = pScene;
}

iGameObject* GameObjectFactory::CreateGameObject(std::string gameObjName, bool isCopy)
{
	iGameObject* pGameObj;

	EntityID entityID = this->m_pScene->GetEntity(gameObjName);

	// Should we use the entity itself or just a copy of it?
	if (isCopy)
	{
		entityID = this->m_pScene->CreateEntity(entityID);
	}

	ScriptComponent* pScript = this->m_pScene->GetComponent<ScriptComponent>(entityID, "script");
	std::string scriptName = pScript->GetScriptName();

	if (scriptName == "GameManager")
	{
		pGameObj = new GameManager();
	}
	else
	{
		// Should not return null
		assert(nullptr);
		return nullptr;
	}

	bool isLoaded = pGameObj->Load(entityID, this->m_pScene);
	if (!isLoaded)
	{
		delete pGameObj;
		return nullptr;
	}

	return pGameObj;
}