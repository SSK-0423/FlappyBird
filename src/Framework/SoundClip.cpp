#include "SoundClip.h"
#include "SoundManager.h"

#include "Editor.h"
#include "Object.h"
#include "imgui.h"

#include "InputSystem.h"

using namespace Utility;

namespace Framework
{
	SoundClip::SoundClip(std::shared_ptr<Object> owner) :
		IComponent(owner), m_soundname(nullptr), m_sourceVoice(nullptr), m_isPaused(false),
		m_restartSamplesPlayed(0), m_isEnd(false), m_samplesPlayed(0), m_totalSamples(0)
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
		m_isEnd = false;
		m_samplesPlayed = 0;
		m_restartSamplesPlayed = 0;
		m_startSamplesPlayed = 0;
		m_totalSamples = 0;

		return RESULT::SUCCESS;
	}
	void SoundClip::Update(float deltaTime)
	{
		if (m_sourceVoice == nullptr)
		{
			return;
		}

		if (InputSystem::GetKeyDown(DIK_SPACE))
		{
			m_sourceVoice->ExitLoop();
			Editor::DebugLog("ExitLoop");
		}
	}
	void SoundClip::Draw()
	{
	}
	void SoundClip::DrawInspector()
	{
		if (ImGui::CollapsingHeader("SoundClip"))
		{
			ImGui::Text("SoundName: %ls", m_soundname);

			if (m_sourceVoice != nullptr)
			{
				XAUDIO2_VOICE_STATE state;
				m_sourceVoice->GetState(&state);
				ImGui::Text("SamplesPlayed: %d", state.SamplesPlayed);
				ImGui::Text("My SamplesPlayed: %d", m_samplesPlayed);
				ImGui::Text("StartSamplesPlayed: %d", m_startSamplesPlayed);
				ImGui::Text("RestartSamplesPlayed: %d", m_restartSamplesPlayed);
				ImGui::Text("BuffersQueued: %d", state.BuffersQueued);
				ImGui::Text("CurrentPlayTime: %f", GetCurrentPlayTime());
				ImGui::Text("IsPaused: %s", m_isPaused ? "true" : "false");
				ImGui::Text("IsEnd: %s", m_isEnd ? "true" : "false");

				SoundData* soundData = SoundManager::GetSoundData(m_soundname);

				if (soundData != nullptr)
				{
					UINT64 samplesPlayed = state.SamplesPlayed;
					DWORD sampleNum = soundData->waveData.audioBytes / soundData->waveData.wfx->nBlockAlign;

					ImGui::Text("SampleNum: %d", sampleNum);
					ImGui::Text("TotalSamples: %d", m_totalSamples);

					//if (m_totalSamples != 0)
					//{
					//	ImGui::Text("SampleNum % TotalSamples: %d", samplesPlayed % m_totalSamples);
					//}
				}
			}
		}
	}
	void SoundClip::Play(float volume, float startTimeSec, bool wait)
	{
		// 一時停止中でない場合は新規にSourceVoiceを取得する
		if (m_isPaused == false)
		{
			m_sourceVoice = SoundManager::Play(m_soundname, this);
		}

		if (m_sourceVoice == nullptr)
		{
			return;
		}

		// 音源をリスタートした際にSamplesPlayedがリセットされないため
		// 再生開始時のSamplesPlayedを記録し、リスタート後でも現在の再生位置を正しく取得できるようにする。
		XAUDIO2_VOICE_STATE state;
		m_sourceVoice->GetState(&state);
		if (startTimeSec == 0.f)
		{
			m_samplesPlayed = 0;
			m_restartSamplesPlayed = state.SamplesPlayed;
		}
		m_startSamplesPlayed = state.SamplesPlayed;

		// 再生位置を設定
		SoundData* soundData = SoundManager::GetSoundData(m_soundname);
		soundData->buffer.PlayBegin = static_cast<UINT32>(soundData->waveData.wfx->nSamplesPerSec * startTimeSec);
		m_sourceVoice->SubmitSourceBuffer(&soundData->buffer);

		// 総サンプル数を記録
		m_totalSamples = soundData->waveData.audioBytes / soundData->waveData.wfx->nBlockAlign;

		// 音量を設定
		m_sourceVoice->SetVolume(volume);

		// サウンドを再生
		m_sourceVoice->Start();

		m_isPaused = false;

		// サウンド管理クラスを作成して、再生中かどうかを判定、管理した方がよいのでは
		if (wait)
		{
			while (!m_isEnd) {}
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

				XAUDIO2_VOICE_STATE state;
				m_sourceVoice->GetState(&state);
				m_samplesPlayed = m_samplesPlayed + (state.SamplesPlayed - m_startSamplesPlayed);
			}
			else
			{
				m_sourceVoice->Stop();
				m_isPaused = false;
			}
			m_sourceVoice->FlushSourceBuffers();

			if (!isPause)
			{
				m_sourceVoice = nullptr;
			}
		}
	}
	void SoundClip::ExitLoop()
	{
		if (m_sourceVoice != nullptr)
		{
			m_sourceVoice->ExitLoop();
		}
	}
	void SoundClip::SetVolume(float volume)
	{
		if (m_sourceVoice != nullptr)
		{
			m_sourceVoice->SetVolume(volume);
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

		INT64 playedSampleNum;
		if (m_isPaused)
		{
			playedSampleNum = m_samplesPlayed;
		}
		else
		{
			playedSampleNum = m_samplesPlayed + (state.SamplesPlayed - m_startSamplesPlayed);// -m_restartSamplesPlayed; //state.SamplesPlayed - m_restartSamplesPlayed;
		}
		DWORD sampleRate = soundData->waveData.wfx->nSamplesPerSec;

		// ループ再生時の再生位置を計算
		// ループ再生時は再生位置がサンプル数を超えるたびに0に戻るため、再生位置をサンプル数で割った余りを取得する。
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

		// 再生位置を変更
		m_samplesPlayed += displacement;

		// 余り算出の前に負の値にならないようにする
		m_samplesPlayed = std::max(m_samplesPlayed, 0LL);

		m_samplesPlayed %= m_totalSamples;

		// 曲の再生範囲を超えないようにする
		m_samplesPlayed = std::clamp(m_samplesPlayed, 0LL, static_cast<INT64>(soundData->waveData.audioBytes / soundData->waveData.wfx->nBlockAlign));
	}
	bool SoundClip::IsEnd()
	{
		return m_isEnd;
	}
	STDMETHODIMP_(void __stdcall) SoundClip::OnVoiceProcessingPassStart(UINT32 BytesRequired)
	{
	}
	STDMETHODIMP_(void __stdcall) SoundClip::OnVoiceProcessingPassEnd()
	{
	}
	STDMETHODIMP_(void __stdcall) SoundClip::OnStreamEnd()
	{
		if (m_sourceVoice != nullptr)
		{
			m_sourceVoice->FlushSourceBuffers();
			m_sourceVoice = nullptr;
			m_isEnd = true;
			OnEnd.Notify(NotificationEvent());
		}
	}
	STDMETHODIMP_(void __stdcall) SoundClip::OnBufferStart(void* pBufferContext)
	{
		Editor::DebugLog("OnBufferStart");
	}
	STDMETHODIMP_(void __stdcall) SoundClip::OnBufferEnd(void* pBufferContext)
	{
		Editor::DebugLog("OnBufferEnd");
	}
	STDMETHODIMP_(void __stdcall) SoundClip::OnLoopEnd(void* pBufferContext)
	{
		Editor::DebugLog("OnLoopEnd");

		// ループ再生時は再生位置をリスタート位置に戻す
		m_samplesPlayed = 0;
	}
	STDMETHODIMP_(void __stdcall) SoundClip::OnVoiceError(void* pBufferContext, HRESULT Error)
	{
	}
}