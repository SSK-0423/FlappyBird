#include "pch.h"
#include "MusicPlayer.h"

#include "imgui.h"

using namespace Framework;
using namespace Utility;

namespace FlappyBird
{
	MusicPlayer::MusicPlayer(std::shared_ptr<Object> owner)
		: Framework::IComponent(owner), m_musicPath(L""), m_isPlaying(false)
	{
		// SoundClip’Ç‰Á
		m_music = m_owner.lock()->AddComponent<SoundClip>(m_owner.lock());
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
		m_music->Stop();

		Utility::charToWchar(musicPath, m_musicPath, _countof(m_musicPath));

		m_music->LoadWavSound(m_musicPath, true);

		Editor::DebugLog("Music Loaded: %s", musicPath);
	}
	void MusicPlayer::Play(float startTimeSec)
	{
		// Ä¶’†‚Ìê‡‚Í‘½dÄ¶‚ð–h‚®‚½‚ß‚É‰½‚à‚µ‚È‚¢
		if (m_isPlaying)
		{
			return;
		}

		float defaultVolume = 0.1f;
		m_music->Play(defaultVolume, startTimeSec);
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
	void MusicPlayer::Seek(float timeSec)
	{
		m_music->Seek(timeSec);
	}
	bool MusicPlayer::IsPlaying()
	{
		return m_isPlaying;
	}
}