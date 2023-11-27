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

		//// �e�X�g�L�����N�^
		//Object* testChara = GameObjectManager::CreateObject();
		//testChara->AddComponent<TestCharacter>(testChara);

		//// �e�X�g��Q��
		//Object* testObstacle = GameObjectManager::CreateObject();
		//testObstacle->AddComponent<TestObstacle>(testObstacle);


		// �w�i
		GameObject* background = GameObjectManager::CreateObject();
		background->AddComponent<Background>(background);

		// �v���C���[
		GameObject* player = GameObjectManager::CreateObject();
		player->AddComponent<Player>(player);

	}
	void GameScene::Final()
	{
		Scene::Final();

		OutputDebugStringA("GameScene Final\n");
	}
}
