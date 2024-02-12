#include "pch.h"
#include "MusicPlayer.h"

#include "imgui.h"

using namespace Framework;
using namespace Utility;

namespace FlappyBird
{
	MusicPlayer::MusicPlayer(std::shared_ptr<Object> owner)
		: Framework::IComponent(owner), m_musicPath(L""), m_isPlaying(false), m_beat(4.f), m_bpm(120.f)
	{
		// SoundClip’Ç‰Á
		m_music = m_owner.lock()->AddComponent<SoundClip>(m_owner.lock());
		m_music->OnEnd.Subscribe([this](NotificationEvent e)
			{
				m_isPlaying = false;
				OnMusicEnd.Notify(NotificationEvent());
			});
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
			ImGui::Text("IsEnd: %s", m_music->IsEnd() ? "true" : "false");
		}
	}
	void MusicPlayer::Load(const std::string& musicPath, bool isLoop)
	{
		m_music->Stop();

		Utility::charToWchar(musicPath.c_str(), m_musicPath, _countof(m_musicPath));

		m_music->LoadWavSound(m_musicPath, isLoop);

		Editor::DebugLog("Music Loaded: %s", musicPath.c_str());
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
		if (m_isPlaying)
		{
			m_music->Stop(true);
			m_isPlaying = false;
		}
	}
	float MusicPlayer::GetMusicLength()
	{
		return m_music->GetLength();
	}
	float MusicPlayer::GetCurrentPlayTimeMs()
	{
		return m_music->GetCurrentPlayTime() * 1000.f;
	}
	void MusicPlayer::Move(float timeSec)
	{
		m_music->Move(timeSec);
	}
	bool MusicPlayer::IsPlaying()
	{
		return m_isPlaying;
	}
	void MusicPlayer::SetVolume(float volume)
	{
		m_music->SetVolume(volume);
	}
	void MusicPlayer::SetBPM(float bpm)
	{
		m_bpm = bpm;
	}
	float MusicPlayer::GetBPM()
	{
		return m_bpm;
	}
	void MusicPlayer::SetBeat(float beat)
	{
		m_beat = beat;
	}
	float MusicPlayer::GetBeat()
	{
		return m_beat;
	}
}