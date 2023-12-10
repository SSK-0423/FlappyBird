#include "pch.h"
#include "GameScene.h"

#include "Framework/GameFramework.h"

#include "GameMaster.h"
#include "Player.h"
#include "Background.h"
#include "ObstacleSpawner.h"
#include "Score.h"

using namespace Framework;

namespace FlappyBird
{
	void GameScene::Init()
	{
		OutputDebugStringA("GameScene Init\n");

		// ���I�u�W�F�N�g��ObjectManager::Find�Ŏ擾����ۂɁA
		// �R���X�g���N�^�ŏ������������s���ƁA�ΏۃI�u�W�F�N�g���܂���������Ă��Ȃ��\�������邽�߁A
		// Unity��Start�֐��̂悤�Ȃ��̂�p�ӂ��A
		// �V�[���̍ŏ��̃t���[�������s�����O�Ɉ�Ăɏ������������s���悤�ɂ���

		// �Q�[���}�X�^�[
		GameObject* gameMaster = GameObjectManager::CreateObject();
		gameMaster->AddComponent<GameMaster>(gameMaster);

		// �w�i
		GameObject* background = GameObjectManager::CreateObject();
		background->AddComponent<Background>(background);

		// �v���C���[
		GameObject* player = GameObjectManager::CreateObject();
		player->AddComponent<Player>(player);

		// ��Q����ݒu����I�u�W�F�N�g
		GameObject* obstacleSpawner = GameObjectManager::CreateObject();
		obstacleSpawner->AddComponent<ObstacleSpawner>(obstacleSpawner);

		// UI
		// �X�R�A
		UIObject* score = UIObjectManager::CreateObject();
		score->AddComponent<Score>(score);
	}
	void GameScene::Final()
	{
		Scene::Final();

		OutputDebugStringA("GameScene Final\n");
	}
}
