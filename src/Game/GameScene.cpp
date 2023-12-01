#include "pch.h"
#include "GameScene.h"

#include "Framework/GameFramework.h"

#include "TestCharacter.h"
#include "TestObstacle.h"

#include "Player.h"
#include "Background.h"

using namespace Framework;

namespace FlappyBird
{
	void GameScene::Init()
	{
		OutputDebugStringA("GameScene Init\n");

		//// テストキャラクタ
		//Object* testChara = GameObjectManager::CreateObject();
		//testChara->AddComponent<TestCharacter>(testChara);

		//// テスト障害物
		//Object* testObstacle = GameObjectManager::CreateObject();
		//testObstacle->AddComponent<TestObstacle>(testObstacle);


		// 背景
		GameObject* background = GameObjectManager::CreateObject();
		background->AddComponent<Background>(background);

		// プレイヤー
		GameObject* player = GameObjectManager::CreateObject();
		player->AddComponent<Player>(player);

	}
	void GameScene::Final()
	{
		Scene::Final();

		OutputDebugStringA("GameScene Final\n");
	}
}
