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

		// ���I�u�W�F�N�g��ObjectManager::Find�Ŏ擾����ۂɁA
		// �R���X�g���N�^�ŏ������������s���ƁA�ΏۃI�u�W�F�N�g���܂���������Ă��Ȃ��\�������邽�߁A
		// Unity��Start�֐��̂悤�Ȃ��̂�p�ӂ��A
		// �V�[���̍ŏ��̃t���[�������s�����O�Ɉ�Ăɏ������������s���悤�ɂ���

		// �Q�[���I�[�o�[UI
		std::shared_ptr<Framework::UIObject> gameOverUI = UIObjectManager::CreateObject();
		gameOverUI->AddComponent<GameOverUI>(gameOverUI);

		// �Q�[���J�n����UI
		std::shared_ptr<Framework::UIObject> gameReadyUI = UIObjectManager::CreateObject();
		gameReadyUI->AddComponent<GameReadyUI>(gameReadyUI);

		// �Q�[���}�X�^�[
		std::shared_ptr<Framework::GameObject> gameMaster = GameObjectManager::CreateObject();
		gameMaster->AddComponent<GameMaster>(gameMaster);

		// �w�i
		std::shared_ptr<Framework::GameObject> background = GameObjectManager::CreateObject();
		background->AddComponent<Background>(background);

		// �v���C���[
		std::shared_ptr<Framework::GameObject> player = GameObjectManager::CreateObject();
		player->AddComponent<Player>(player);

		// ��Q����ݒu����I�u�W�F�N�g
		std::shared_ptr<Framework::GameObject> notesManagerObj = GameObjectManager::CreateObject();
		notesManagerObj->SetName("NotesManager");
		notesManagerObj->AddComponent<NotesManager>(notesManagerObj);

		// �ȍĐ��I�u�W�F�N�g
		std::shared_ptr<Framework::GameObject> musicPlayerObj = GameObjectManager::CreateObject();
		musicPlayerObj->SetName("MusicPlayer");
		musicPlayerObj->AddComponent<MusicPlayer>(musicPlayerObj);

		// UI
		std::shared_ptr<Framework::UIObject> scoreFrame = UIObjectManager::CreateObject();
		scoreFrame->AddComponent<ScoreFrame>(scoreFrame);

		// �X�R�A
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
