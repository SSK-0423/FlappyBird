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

		// �w�i
		GameObject* background = GameObjectManager::CreateObject();
		background->AddComponent<Background>(background);

		// �v���C���[
		GameObject* player = GameObjectManager::CreateObject();
		player->AddComponent<Player>(player);

		// ��Q����ݒu����I�u�W�F�N�g
		GameObject* obstacleSpawner = GameObjectManager::CreateObject();
		obstacleSpawner->AddComponent<ObstacleSpawner>(obstacleSpawner);

		// �Q�[���}�X�^�[
		GameObject* gameMaster = GameObjectManager::CreateObject();
		gameMaster->AddComponent<GameMaster>(gameMaster);
	}
	void GameScene::Final()
	{
		Scene::Final();

		OutputDebugStringA("GameScene Final\n");
	}
}
