#include "SoundClip.h"
#include "SoundManager.h"

#include "Editor.h"

#include "imgui.h"

namespace Framework
{
	SoundClip::SoundClip(Object* owner) :
		IComponent(owner), m_soundname(nullptr), m_sourceVoice(nullptr), m_isPaused(false), m_isPlaying(false)
	{
	}
	SoundClip::~SoundClip()
	{
		Utility::DebugLog("SoundClip::~SoundClip()\n");
	}
	Utility::RESULT SoundClip::LoadWavSound(const wchar_t* filename, bool isLoop)
	{
		m_soundname = filename;
		return SoundManager::LoadWavSound(filename, isLoop);
	}
	void SoundClip::Update(float deltaTime)
	{
	}
	void SoundClip::Draw()
	{
	}
	void SoundClip::DrawInspector()
	{
		if (ImGui::CollapsingHeader("SoundClip"))
		{
			ImGui::Text("SoundName: %s", m_soundname);
			ImGui::Text("IsPlaying: %s", m_isPlaying ? "true" : "false");
			ImGui::Text("IsPaused: %s", m_isPaused ? "true" : "false");
		}
	}
	void SoundClip::Play(float volume, bool wait)
	{
		// 一時停止中でない場合は新規にSourceVoiceを取得する
		if (m_isPaused == false)
		{
			m_sourceVoice = SoundManager::Play(m_soundname);
		}

		if (m_sourceVoice == nullptr)
		{
			return;
		}
		m_sourceVoice->SetVolume(volume);

		// 再生中かどうかを判定する必要がある
		// 直ぐにデリートされては困るので、一度再生が終わるまで待つ
		m_sourceVoice->Start();

		// サウンド管理クラスを作成して、再生中かどうかを判定、管理した方がよいのでは
		if (wait)
		{
			XAUDIO2_VOICE_STATE state;
			m_sourceVoice->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
			while (state.BuffersQueued > 0)
			{
				m_sourceVoice->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
			}
		}

		//Utility::DebugLog("再生終了\n");
	}
	void SoundClip::Stop(bool isPause)
	{
		if (m_sourceVoice != nullptr)
		{
			if (isPause)
			{
				m_sourceVoice->Stop(XAUDIO2_PLAY_TAILS);
				m_isPaused = true;
			}
			else
			{
				m_sourceVoice->Stop();
				m_isPaused = false;
			}
		}
	}
	float SoundClip::GetLength()
	{
		if (m_soundname == nullptr)
		{
			return 0.0f;
		}

		SoundData* soundData = SoundManager::GetSoundData(m_soundname);

		if (soundData == nullptr)
		{
			return 0.0f;
		}

		// サンプルレートとサンプル数から長さを計算
		DWORD sampleNum = soundData->waveData.audioBytes / soundData->waveData.wfx->nBlockAlign;
		DWORD sampleRate = soundData->waveData.wfx->nSamplesPerSec;

		return (float)sampleNum / sampleRate;
	}
	float SoundClip::GetCurrentPlayTime()
	{
		if (m_sourceVoice == nullptr)
		{
			return -1.f;
		}

		SoundData* soundData = SoundManager::GetSoundData(m_soundname);

		if (soundData == nullptr)
		{
			return -1.f;
		}

		XAUDIO2_VOICE_STATE state;
		m_sourceVoice->GetState(&state);

		// 再生済みのサンプル数とサンプルレートから現在の再生位置を計算
		UINT64 playedSampleNum = state.SamplesPlayed;
		DWORD sampleRate = soundData->waveData.wfx->nSamplesPerSec;

		return (float)playedSampleNum / sampleRate;
	}
}