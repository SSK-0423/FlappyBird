#include "pch.h"
#include "GameMaster.h"
#include "Player.h"

using namespace Framework;

namespace FlappyBird
{
	GameMaster::GameMaster(Framework::Object* owner) : IComponent(owner)
	{
		// BGM�ǉ�
		SoundClip* sound = m_owner->AddComponent<SoundClip>(m_owner);
		sound->LoadWavSound(L"res/sound/������ ���[�v  8bit05.wav");
		sound->Play();

		// �v���C���[�ǉ�
		m_player = ObjectManager<GameObject>::FindObject("Player")->GetComponent<Player>();
	}
	GameMaster::~GameMaster()
	{
	}
	void GameMaster::Update(float deltaTime)
	{
		// �v���C���[�����S������BGM���~�߂�
		if (m_player->IsDead())
		{
			m_owner->GetComponent<SoundClip>()->Stop();
		}
	}
	void GameMaster::Draw()
	{
	}
}

