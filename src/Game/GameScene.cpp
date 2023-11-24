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

		// テストキャラクタ
		Object* testChara = GameObjectManager::CreateObject();
		testChara->AddComponent<TestCharacter>(testChara);

		// テスト障害物
		Object* testObstacle = GameObjectManager::CreateObject();
		testObstacle->AddComponent<TestObstacle>(testObstacle);
	}
	void GameScene::Final()
	{
		Scene::Final();

		OutputDebugStringA("GameScene Final\n");
	}
}
