#include "pch.h"
#include "GameScene.h"

#include "Framework/GameFramework.h"

#include "GameMaster.h"
#include "Player.h"
#include "Background.h"

#include "NotesManager.h"
#include "MusicPlayer.h"

#include "Score.h"
#include "ScoreFrame.h"

#include "GameReadyUI.h"
#include "GameOverUI.h"

using namespace Framework;

namespace FlappyBird
{
	void GameScene::Init()
	{
#ifdef _DEBUG
		Editor::DebugLog("GameScene Init\n");
#endif // _DEBUG

		// 他オブジェクトをObjectManager::Findで取得する際に、
		// コンストラクタで初期化処理を行うと、対象オブジェクトがまだ生成されていない可能性があるため、
		// UnityのStart関数のようなものを用意し、
		// シーンの最初のフレームが実行される前に一斉に初期化処理を行うようにする

		// ゲームオーバーUI
		std::shared_ptr<Framework::UIObject> gameOverUI = UIObjectManager::CreateObject();
		gameOverUI->AddComponent<GameOverUI>(gameOverUI);

		// ゲーム開始準備UI
		std::shared_ptr<Framework::UIObject> gameReadyUI = UIObjectManager::CreateObject();
		gameReadyUI->AddComponent<GameReadyUI>(gameReadyUI);

		// ゲームマスター
		std::shared_ptr<Framework::GameObject> gameMaster = GameObjectManager::CreateObject();
		gameMaster->AddComponent<GameMaster>(gameMaster);

		// 背景
		std::shared_ptr<Framework::GameObject> background = GameObjectManager::CreateObject();
		background->AddComponent<Background>(background);

		// プレイヤー
		std::shared_ptr<Framework::GameObject> player = GameObjectManager::CreateObject();
		player->AddComponent<Player>(player);

		// 障害物を設置するオブジェクト
		std::shared_ptr<Framework::GameObject> notesManagerObj = GameObjectManager::CreateObject();
		notesManagerObj->SetName("NotesManager");
		notesManagerObj->AddComponent<NotesManager>(notesManagerObj);

		// 曲再生オブジェクト
		std::shared_ptr<Framework::GameObject> musicPlayerObj = GameObjectManager::CreateObject();
		musicPlayerObj->SetName("MusicPlayer");
		musicPlayerObj->AddComponent<MusicPlayer>(musicPlayerObj);

		// UI
		std::shared_ptr<Framework::UIObject> scoreFrame = UIObjectManager::CreateObject();
		scoreFrame->AddComponent<ScoreFrame>(scoreFrame);

		// スコア
		std::shared_ptr<Framework::UIObject> score = UIObjectManager::CreateObject();
		score->AddComponent<Score>(score);
	}
	void GameScene::Final()
	{
		Scene::Final();
#ifdef _DEBUG
		Editor::DebugLog("GameScene Final\n");
#endif // _DEBUG
	}
}
