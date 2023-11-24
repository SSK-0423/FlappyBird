#include "pch.h"
#include "GameScene.h"

#include "Framework/GameFramework.h"

#include "TestCharacter.h"
#include "TestObstacle.h"

using namespace Framework;

namespace FlappyBird
{
	void GameScene::Init()
	{
		OutputDebugStringA("GameScene Init\n");

		// �e�X�g�L�����N�^
		Object* testChara = GameObjectManager::CreateObject();
		testChara->AddComponent<TestCharacter>(testChara);

		// �e�X�g��Q��
		Object* testObstacle = GameObjectManager::CreateObject();
		testObstacle->AddComponent<TestObstacle>(testObstacle);
	}
	void GameScene::Final()
	{
		Scene::Final();

		OutputDebugStringA("GameScene Final\n");
	}
}
