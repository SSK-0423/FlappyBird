#include "pch.h"
#include "GameMaster.h"
#include "Player.h"
#include "GameOverUI.h"
#include "GameReadyUI.h"

#include "FumenJsonReadWriter.h"
#include "NotesManager.h"
#include "HiddenNotesManager.h"
#include "MusicPlayer.h"

using namespace Framework;

namespace FlappyBird
{
	GameMaster::GameMaster(std::shared_ptr<Framework::Object> owner) :
		IComponent(owner), m_gameState(GAME_STATE::READY), m_elapsedTime(0.0f), m_gameStartTime(4.0f)
	{
		m_gameOverUI = UIObjectManager::FindObject("GameOverUI");
		m_gameReadyUI = UIObjectManager::FindObject("GameReadyUI");
		m_gameClearUI = UIObjectManager::FindObject("GameClearUI");

		m_owner.lock()->SetName("GameMaster");
	}
	void GameMaster::Start()
	{
		// ���ʓǂݍ���
		FumenData fumenData;

		// TODO: �ȑI���V�[���őI�����ꂽ�Ȃ̕��ʂ�ǂݍ���
		FumenJsonReadWriter::Read("res/fumen/Froggy's_Song.json", fumenData);
		//FumenJsonReadWriter::Read("res/fumen/SaturnEbiMan.json", fumenData);
		//FumenJsonReadWriter::Read("res/fumen/LoopTest.json", fumenData);

		// �m�[�c�̐���
		std::shared_ptr<Framework::GameObject> notesManagerObj = GameObjectManager::FindObject("NotesManager");
		notesManagerObj->GetComponent<NotesManager>()->SetNotes(fumenData.noteDatas);

		// �ȓǂݍ���
		std::shared_ptr<Framework::GameObject> musicPlayerObj = GameObjectManager::FindObject("MusicPlayer");
		MusicPlayer* musicPlayer = musicPlayerObj->GetComponent<MusicPlayer>();
		musicPlayer->Load(fumenData.musicFilePath, false);
		musicPlayer->SetBPM(fumenData.bpm);
		musicPlayer->SetBeat(fumenData.beat);
		musicPlayer->OnMusicEnd.Subscribe([this](NotificationEvent e) { ChangeState(GAME_STATE::GAMECLEAR); });

		// �B���m�[�c�̐���
		float musicLength = musicPlayer->GetMusicLength();
		unsigned int barNum = musicLength * fumenData.bpm / (60.f * fumenData.beat);
		std::shared_ptr<Framework::GameObject> hiddenNotesManagerObj = GameObjectManager::FindObject("HiddenNotesManager");
		hiddenNotesManagerObj->GetComponent<HiddenNotesManager>()->CreateHiddenNotes(barNum, fumenData.bpm, fumenData.beat);
	}
	void GameMaster::Update(float deltaTime)
	{
		switch (m_gameState)
		{
		case GAME_STATE::READY:
			GameReady(deltaTime);
			break;
		case GAME_STATE::PLAYING:
			break;
		case GAME_STATE::GAMEOVER:
			GameOver(deltaTime);
			break;
		default:
			break;
		}
	}
	void GameMaster::Draw()
	{
	}
	void GameMaster::ChangeState(GAME_STATE state)
	{
		m_gameState = state;

		// �X�e�[�g�؂�ւ����̏���
		switch (m_gameState)
		{
		case GAME_STATE::READY:
			OnGameReady();
			break;
		case GAME_STATE::PLAYING:
			OnGameStart();
			break;
		case GAME_STATE::GAMECLEAR:
			OnGameClear();
			break;
		case GAME_STATE::GAMEOVER:
			OnGameOver();
			break;
		default:
			break;
		}
	}
	GAME_STATE GameMaster::GetGameState()
	{
		return m_gameState;
	}
	void GameMaster::GameReady(float deltaTime)
	{
		m_elapsedTime += deltaTime;
		if (m_elapsedTime >= m_gameStartTime)
		{
			// �R�[���o�b�N���ĂԂ��߂ɒ��ڑ���ł͂Ȃ��֐����Ă�
			ChangeState(GAME_STATE::PLAYING);
		}
	}
	void GameMaster::GameOver(float deltaTime)
	{
		// �Q�[���I�[�o�[UI��\��
	}
	void GameMaster::OnGameReady()
	{
		// �Q�[���J�nUI��\��
		m_gameReadyUI->SetActive(true);
	}
	void GameMaster::OnGameStart()
	{
		// �Q�[���J�nUI���\��
		m_gameReadyUI->SetActive(false);

		// �Ȃ̍Đ����J�n
		GameObjectManager::FindObject("MusicPlayer")->GetComponent<MusicPlayer>()->Play();
	}
	void GameMaster::OnGameClear()
	{
		// �Q�[���N���AUI��\��
		m_gameClearUI->SetActive(true);

		// �Ȃ̍Đ����~
		GameObjectManager::FindObject("MusicPlayer")->GetComponent<MusicPlayer>()->Stop();

		// �m�[�c�̕`����~
		GameObjectManager::FindObject("NotesManager")->GetComponent<NotesManager>()->SetActive(false);
	}
	void GameMaster::OnGameOver()
	{
		// �Q�[���I�[�o�[UI��\��
		m_gameOverUI->SetActive(true);

		// �Ȃ̍Đ����~
		GameObjectManager::FindObject("MusicPlayer")->GetComponent<MusicPlayer>()->Stop();

		// �m�[�c�̈ړ����~
		GameObjectManager::FindObject("NotesManager")->GetComponent<NotesManager>()->SetActive(false);
	}
}