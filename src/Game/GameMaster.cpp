#include "pch.h"
#include "GameMaster.h"
#include "Player.h"

using namespace Framework;

namespace FlappyBird
{
	GameMaster::GameMaster(Framework::Object* owner) :
		IComponent(owner), m_gameState(GAME_STATE::READY), m_elapsedTime(0.0f), m_gameStartTime(4.0f)
	{
		m_owner->SetName("GameMaster");

		// BGM追加
		SoundClip* sound = m_owner->AddComponent<SoundClip>(m_owner);
		sound->LoadWavSound(L"res/sound/魔王魂 ループ  8bit05.wav");
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
	}
	void GameMaster::GameOver(float deltaTime)
	{
		m_owner->GetComponent<SoundClip>()->Stop();

		// ゲームオーバーUIを表示
	}
}

