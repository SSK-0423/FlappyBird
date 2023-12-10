#include "pch.h"
#include "GameMaster.h"
#include "Player.h"

using namespace Framework;

namespace FlappyBird
{
	GameMaster::GameMaster(Framework::Object* owner) : IComponent(owner)
	{
		// BGM追加
		SoundClip* sound = m_owner->AddComponent<SoundClip>(m_owner);
		sound->LoadWavSound(L"res/sound/魔王魂 ループ  8bit05.wav");
		sound->Play();

		// プレイヤー追加
		m_player = ObjectManager<GameObject>::FindObject("Player")->GetComponent<Player>();
	}
	GameMaster::~GameMaster()
	{
	}
	void GameMaster::Update(float deltaTime)
	{
		// プレイヤーが死亡したらBGMを止める
		if (m_player->IsDead())
		{
			m_owner->GetComponent<SoundClip>()->Stop();
		}
	}
	void GameMaster::Draw()
	{
	}
}

