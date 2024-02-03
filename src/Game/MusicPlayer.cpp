#include "pch.h"
#include "MusicPlayer.h"

using namespace Framework;
using namespace Utility;

namespace FlappyBird
{
	MusicPlayer::MusicPlayer(Framework::Object* owner)
		: Framework::IComponent(owner), m_musicPath(L""), m_isPlaying(false)
	{
		// SoundClip追加
		m_music = m_owner->AddComponent<SoundClip>(m_owner);
	}
	MusicPlayer::~MusicPlayer()
	{
	}
	void MusicPlayer::Update(float deltaTime)
	{
	}
	void MusicPlayer::Draw()
	{
	}
	void MusicPlayer::Load(const char* musicPath)
	{
		// パスをwchar_t型に変換
		Utility::charToWchar(musicPath, m_musicPath, 256);

		m_music->LoadWavSound(m_musicPath);

		m_isPlaying = false;
	}
	void MusicPlayer::Play()
	{
		// 再生中の場合は多重再生を防ぐために何もしない
		if (m_isPlaying)
		{
			return;
		}

		m_music->Play();
		m_isPlaying = true;
	}
	void MusicPlayer::Stop()
	{
		m_music->Stop(true);
		m_isPlaying = false;
	}
}