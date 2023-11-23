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
		std::shared_ptr<Object> testChara = std::make_shared<Object>();
		testChara->AddComponent<TestCharacter>(testChara.get());
		ObjectManager::AddObject(testChara);

		// テスト障害物
		std::shared_ptr<Object> testObstacle = std::make_shared<Object>();
		testObstacle->AddComponent<TestObstacle>(testObstacle.get());
		ObjectManager::AddObject(testObstacle);
	}
	void GameScene::Final()
	{
		for (auto& canvas : m_canvases)
		{
			canvas->Final();
		}
		m_canvases.clear();
		m_canvases.shrink_to_fit();

		OutputDebugStringA("GameScene Final\n");
	}
}
