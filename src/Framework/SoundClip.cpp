#include "SoundClip.h"
#include "SoundManager.h"

#include "Editor.h"

#include "imgui.h"

using namespace Utility;

namespace Framework
{
	SoundClip::SoundClip(Object* owner) :
		IComponent(owner), m_soundname(nullptr), m_sourceVoice(nullptr), m_isPaused(false),
		m_restartSamplesPlayed(0)
	{
	}
	SoundClip::~SoundClip()
	{
	}
	Utility::RESULT SoundClip::LoadWavSound(const wchar_t* filename, bool isLoop)
	{
		if (SoundManager::LoadWavSound(filename, isLoop) == RESULT::FAILED)
		{
			return RESULT::FAILED;
		}

		m_soundname = filename;
		m_isPaused = false;

		//m_sourceVoice = SoundManager::Play(m_soundname);
		//m_sourceVoice->Stop();

		return RESULT::SUCCESS;
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
			ImGui::Text("IsPaused: %s", m_isPaused ? "true" : "false");

			if (m_sourceVoice != nullptr)
			{
				XAUDIO2_VOICE_STATE state;
				m_sourceVoice->GetState(&state);
				ImGui::Text("SamplesPlayed: %d", state.SamplesPlayed);
			}
		}
	}
	void SoundClip::Play(float volume, float startTimeSec, bool wait)
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

		// 音源をリスタートした際にSamplesPlayedがリセットされないため
		// 再生開始時のSamplesPlayedを記録し、リスタート後でも現在の再生位置を正しく取得できるようにする。
		if (startTimeSec == 0.f)
		{
			XAUDIO2_VOICE_STATE state;
			m_sourceVoice->GetState(&state);
			m_restartSamplesPlayed = state.SamplesPlayed;
		}

		// 再生位置を設定
		SoundData* soundData = SoundManager::GetSoundData(m_soundname);
		soundData->buffer.PlayBegin = static_cast<UINT32>(soundData->waveData.wfx->nSamplesPerSec * startTimeSec);
		m_sourceVoice->SubmitSourceBuffer(&soundData->buffer);

		// 音量を設定
		m_sourceVoice->SetVolume(volume);

		// サウンドを再生
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
			m_sourceVoice->FlushSourceBuffers();

			// 状態を取得して表示
			XAUDIO2_VOICE_STATE state;
			m_sourceVoice->GetState(&state);
			Editor::DebugLog("SamplesPlayed: %d", state.SamplesPlayed);
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
		// リスタートした際の再生位置を考慮を減算することで、正確な再生位置を取得できるようにする。
		UINT64 playedSampleNum = state.SamplesPlayed - m_restartSamplesPlayed;
		DWORD sampleRate = soundData->waveData.wfx->nSamplesPerSec;

		// ループ再生時の再生位置を計算
		playedSampleNum %= soundData->waveData.audioBytes / soundData->waveData.wfx->nBlockAlign;

		return (float)playedSampleNum / sampleRate;
	}
	void SoundClip::Seek(float timeSec)
	{
		if (m_sourceVoice == nullptr)
		{
			return;
		}

		SoundData* soundData = SoundManager::GetSoundData(m_soundname);

		if (soundData == nullptr)
		{
			return;
		}

		// 現在の再生位置を設定
		DWORD sampleRate = soundData->waveData.wfx->nSamplesPerSec;
		INT64 displacement = sampleRate * timeSec;

		XAUDIO2_VOICE_STATE state;
		m_sourceVoice->GetState(&state);

		// 曲を進める時は値をマイナスする
		// 曲を戻す時は値をプラスする
		INT64 newRestartSamplesPlayed = m_restartSamplesPlayed - displacement;
		
		// オーバーフローを防ぐ
		if (state.SamplesPlayed > newRestartSamplesPlayed)
		{
			m_restartSamplesPlayed = newRestartSamplesPlayed;
		}
	}
}