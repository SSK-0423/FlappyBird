#include "pch.h"
#include "GameMaster.h"
#include "Player.h"
#include "GameOverUI.h"

using namespace Framework;

namespace FlappyBird
{
	GameMaster::GameMaster(Framework::Object* owner) :
		IComponent(owner), m_gameState(GAME_STATE::READY), m_elapsedTime(0.0f), m_gameStartTime(4.0f)
	{
		m_gameOverUI = UIObjectManager::FindObject("GameOverUI");

		m_owner->SetName("GameMaster");

		// BGM�ǉ�
		SoundClip* sound = m_owner->AddComponent<SoundClip>(m_owner);
		sound->LoadWavSound(L"res/sound/������ ���[�v  8bit05.wav");
		sound->Play();
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
			break;
		case GAME_STATE::PLAYING:
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
			m_gameState = GAME_STATE::PLAYING;
		}

		//Utility::DebugLog("Game Ready\n");
	}
	void GameMaster::GameOver(float deltaTime)
	{
		m_owner->GetComponent<SoundClip>()->Stop();

		// �Q�[���I�[�o�[UI��\��
	}
	void GameMaster::OnGameOver()
	{
		// �Q�[���I�[�o�[UI��\��
		m_gameOverUI->SetActive(true);
	}
}

