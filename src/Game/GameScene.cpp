#include "pch.h"
#include "GameScene.h"

#include "Framework/GameFramework.h"

#include "TestCharacter.h"
#include "TestObstacle.h"

#include "GameMaster.h"
#include "Player.h"
#include "Background.h"
#include "ObstacleSpawner.h"

using namespace Framework;

namespace FlappyBird
{
	void GameScene::Init()
	{
		OutputDebugStringA("GameScene Init\n");

		// 背景
		GameObject* background = GameObjectManager::CreateObject();
		background->AddComponent<Background>(background);

		// プレイヤー
		GameObject* player = GameObjectManager::CreateObject();
		player->AddComponent<Player>(player);

		// 障害物を設置するオブジェクト
		GameObject* obstacleSpawner = GameObjectManager::CreateObject();
		obstacleSpawner->AddComponent<ObstacleSpawner>(obstacleSpawner);

		// ゲームマスター
		GameObject* gameMaster = GameObjectManager::CreateObject();
		gameMaster->AddComponent<GameMaster>(gameMaster);
	}
	void GameScene::Final()
	{
		Scene::Final();

		OutputDebugStringA("GameScene Final\n");
	}
}
