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
		// �ꎞ��~���łȂ��ꍇ�͐V�K��SourceVoice���擾����
		if (m_isPaused == false)
		{
			m_sourceVoice = SoundManager::Play(m_soundname, this);
		}

		if (m_sourceVoice == nullptr)
		{
			return;
		}

		// ���������X�^�[�g�����ۂ�SamplesPlayed�����Z�b�g����Ȃ�����
		// �Đ��J�n����SamplesPlayed���L�^���A���X�^�[�g��ł����݂̍Đ��ʒu�𐳂����擾�ł���悤�ɂ���B
		XAUDIO2_VOICE_STATE state;
		m_sourceVoice->GetState(&state);
		if (startTimeSec == 0.f)
		{
			m_samplesPlayed = 0;
			m_restartSamplesPlayed = state.SamplesPlayed;
		}
		m_startSamplesPlayed = state.SamplesPlayed;

		// �Đ��ʒu��ݒ�
		SoundData* soundData = SoundManager::GetSoundData(m_soundname);
		soundData->buffer.PlayBegin = static_cast<UINT32>(soundData->waveData.wfx->nSamplesPerSec * startTimeSec);
		m_sourceVoice->SubmitSourceBuffer(&soundData->buffer);

		// ���T���v�������L�^
		m_totalSamples = soundData->waveData.audioBytes / soundData->waveData.wfx->nBlockAlign;

		// ���ʂ�ݒ�
		m_sourceVoice->SetVolume(volume);

		// �T�E���h���Đ�
		m_sourceVoice->Start();

		m_isPaused = false;

		// �T�E���h�Ǘ��N���X���쐬���āA�Đ������ǂ����𔻒�A�Ǘ����������悢�̂ł�
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

		// �T���v�����[�g�ƃT���v�������璷�����v�Z
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

		// �Đ��ς݂̃T���v�����ƃT���v�����[�g���猻�݂̍Đ��ʒu���v�Z
		// ���X�^�[�g�����ۂ̍Đ��ʒu���l�������Z���邱�ƂŁA���m�ȍĐ��ʒu���擾�ł���悤�ɂ���B

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

		// ���[�v�Đ����̍Đ��ʒu���v�Z
		// ���[�v�Đ����͍Đ��ʒu���T���v�����𒴂��邽�т�0�ɖ߂邽�߁A�Đ��ʒu���T���v�����Ŋ������]����擾����B
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

		// ���݂̍Đ��ʒu��ݒ�
		DWORD sampleRate = soundData->waveData.wfx->nSamplesPerSec;
		INT64 displacement = sampleRate * timeSec;

		// �Đ��ʒu��ύX
		m_samplesPlayed += displacement;

		// �]��Z�o�̑O�ɕ��̒l�ɂȂ�Ȃ��悤�ɂ���
		m_samplesPlayed = std::max(m_samplesPlayed, 0LL);

		m_samplesPlayed %= m_totalSamples;

		// �Ȃ̍Đ��͈͂𒴂��Ȃ��悤�ɂ���
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

		// ���[�v�Đ����͍Đ��ʒu�����X�^�[�g�ʒu�ɖ߂�
		m_samplesPlayed = 0;
	}
	STDMETHODIMP_(void __stdcall) SoundClip::OnVoiceError(void* pBufferContext, HRESULT Error)
	{
	}
}