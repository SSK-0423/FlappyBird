#include "pch.h"
#include "GameMaster.h"
#include "Player.h"
#include "GameOverUI.h"
#include "GameReadyUI.h"

using namespace Framework;

namespace FlappyBird
{
	GameMaster::GameMaster(Framework::Object* owner) :
		IComponent(owner), m_gameState(GAME_STATE::READY), m_elapsedTime(0.0f), m_gameStartTime(4.0f)
	{
		m_gameOverUI = UIObjectManager::FindObject("GameOverUI");
		m_gameReadyUI = UIObjectManager::FindObject("GameReadyUI");

		m_owner->SetName("GameMaster");

		// BGM追加
		SoundClip* sound = m_owner->AddComponent<SoundClip>(m_owner);
		sound->LoadWavSound(L"res/sound/game_bgm.wav");
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

		// ステート切り替え時の処理
		switch (m_gameState)
		{
		case GAME_STATE::READY:
			OnGameReady();
			break;
		case GAME_STATE::PLAYING:
			OnGameStart();
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
			// コールバックを呼ぶために直接代入ではなく関数を呼ぶ
			ChangeState(GAME_STATE::PLAYING);
		}

		//Utility::DebugLog("%f\n", deltaTime);
		//Utility::DebugLog("Game Ready\n");
	}
	void GameMaster::GameOver(float deltaTime)
	{
		m_owner->GetComponent<SoundClip>()->Stop();

		// ゲームオーバーUIを表示
	}
	void GameMaster::OnGameReady()
	{
		// ゲーム開始UIを表示
		m_gameReadyUI->SetActive(true);
	}
	void GameMaster::OnGameStart()
	{
		// ゲーム開始UIを非表示
		m_gameReadyUI->SetActive(false);
	}
	void GameMaster::OnGameOver()
	{
		// ゲームオーバーUIを表示
		m_gameOverUI->SetActive(true);
	}
}

