#include "pch.h"
#include "GameScene.h"

#include "Framework/GameFramework.h"

#include "GameMaster.h"
#include "Player.h"
#include "Background.h"
#include "ObstacleSpawner.h"
#include "Score.h"
#include "ScoreFrame.h"
#include "GameReadyUI.h"
#include "GameOverUI.h"

using namespace Framework;

namespace FlappyBird
{
	void GameScene::Init()
	{
		OutputDebugStringA("GameScene Init\n");

		// 他オブジェクトをObjectManager::Findで取得する際に、
		// コンストラクタで初期化処理を行うと、対象オブジェクトがまだ生成されていない可能性があるため、
		// UnityのStart関数のようなものを用意し、
		// シーンの最初のフレームが実行される前に一斉に初期化処理を行うようにする

		// ゲームオーバーUI
		UIObject* gameOverUI = UIObjectManager::CreateObject();
		gameOverUI->AddComponent<GameOverUI>(gameOverUI);

		// ゲーム開始準備UI
		UIObject* gameReadyUI = UIObjectManager::CreateObject();
		gameReadyUI->AddComponent<GameReadyUI>(gameReadyUI);

		// ゲームマスター
		GameObject* gameMaster = GameObjectManager::CreateObject();
		gameMaster->AddComponent<GameMaster>(gameMaster);

		// 背景
		GameObject* background = GameObjectManager::CreateObject();
		background->AddComponent<Background>(background);

		// プレイヤー
		GameObject* player = GameObjectManager::CreateObject();
		player->AddComponent<Player>(player);

		// 障害物を設置するオブジェクト
		GameObject* obstacleSpawner = GameObjectManager::CreateObject();
		obstacleSpawner->AddComponent<ObstacleSpawner>(obstacleSpawner);

		// UI
		UIObject* scoreFrame = UIObjectManager::CreateObject();
		scoreFrame->AddComponent<ScoreFrame>(scoreFrame);

		// スコア
		UIObject* score = UIObjectManager::CreateObject();
		score->AddComponent<Score>(score);
	}
	void GameScene::Final()
	{
		Scene::Final();

		OutputDebugStringA("GameScene Final\n");
	}
}
