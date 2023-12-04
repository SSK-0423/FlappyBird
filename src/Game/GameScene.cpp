#include "pch.h"
#include "GameScene.h"

#include "Framework/GameFramework.h"

#include "TestCharacter.h"
#include "TestObstacle.h"

#include "Player.h"
#include "Obstacle.h"
#include "Background.h"
#include "ObstaclePool.h"

using namespace Framework;

namespace FlappyBird
{
	void GameScene::Init()
	{
		OutputDebugStringA("GameScene Init\n");

		// �w�i
		GameObject* background = GameObjectManager::CreateObject();
		background->AddComponent<Background>(background);

		// �v���C���[
		GameObject* player = GameObjectManager::CreateObject();
		player->AddComponent<Player>(player);

		// ��Q���v�[��
		GameObject* obstaclePool = GameObjectManager::CreateObject();
		obstaclePool->AddComponent<ObstaclePool>(obstaclePool);

		obstaclePool->GetComponent<ObstaclePool>()->GetObstacle();
	}
	void GameScene::Final()
	{
		Scene::Final();

		OutputDebugStringA("GameScene Final\n");
	}
}
