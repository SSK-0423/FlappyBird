#include "pch.h"
#include "MusicPlayer.h"

#include "imgui.h"

using namespace Framework;
using namespace Utility;

namespace FlappyBird
{
	MusicPlayer::MusicPlayer(Framework::Object* owner)
		: Framework::IComponent(owner), m_musicPath(L""), m_isPlaying(false)
	{
		// SoundClip’Ç‰Á
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
	void MusicPlayer::DrawInspector()
	{
		if (ImGui::CollapsingHeader("MusicPlayer"))
		{
			ImGui::Text("MusicPath: %s", m_musicPath);
			ImGui::Text("IsPlaying: %s", m_isPlaying ? "true" : "false");
			ImGui::Text("MusicLength: %.2f", m_music->GetLength());
			ImGui::Text("CurrentTime: %.2f", m_music->GetCurrentPlayTime());
		}
	}
	void MusicPlayer::Load(const char* musicPath)
	{
		Utility::charToWchar(musicPath, m_musicPath, _countof(m_musicPath));

		m_music->LoadWavSound(m_musicPath, true);

		m_isPlaying = false;
	}
	void MusicPlayer::Play()
	{
		// Ä¶’†‚Ìê‡‚Í‘½dÄ¶‚ð–h‚®‚½‚ß‚É‰½‚à‚µ‚È‚¢
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
	float MusicPlayer::GetCurrentPlayTimeMs()
	{
		return m_music->GetCurrentPlayTime() * 1000.f;
	}
}