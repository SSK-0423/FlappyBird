#include "pch.h"
#include "MusicPlayer.h"

using namespace Framework;
using namespace Utility;

namespace FlappyBird
{
	MusicPlayer::MusicPlayer(Framework::Object* owner)
		: Framework::IComponent(owner), m_musicPath(L""), m_isPlaying(false)
	{
		// SoundClip�ǉ�
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
		// �p�X��wchar_t�^�ɕϊ�
		Utility::charToWchar(musicPath, m_musicPath, _countof(m_musicPath));

		m_music->LoadWavSound(m_musicPath);

		m_isPlaying = false;
	}
	void MusicPlayer::Play()
	{
		// �Đ����̏ꍇ�͑��d�Đ���h�����߂ɉ������Ȃ�
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
	float MusicPlayer::GetMusicLength()
	{
		return m_music->GetLength();
	}
}