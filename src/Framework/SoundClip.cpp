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
		// �ꎞ��~���łȂ��ꍇ�͐V�K��SourceVoice���擾����
		if (m_isPaused == false)
		{
			m_sourceVoice = SoundManager::Play(m_soundname);
		}

		if (m_sourceVoice == nullptr)
		{
			return;
		}

		// ���������X�^�[�g�����ۂ�SamplesPlayed�����Z�b�g����Ȃ�����
		// �Đ��J�n����SamplesPlayed���L�^���A���X�^�[�g��ł����݂̍Đ��ʒu�𐳂����擾�ł���悤�ɂ���B
		if (startTimeSec == 0.f)
		{
			XAUDIO2_VOICE_STATE state;
			m_sourceVoice->GetState(&state);
			m_restartSamplesPlayed = state.SamplesPlayed;
		}

		// �Đ��ʒu��ݒ�
		SoundData* soundData = SoundManager::GetSoundData(m_soundname);
		soundData->buffer.PlayBegin = static_cast<UINT32>(soundData->waveData.wfx->nSamplesPerSec * startTimeSec);
		m_sourceVoice->SubmitSourceBuffer(&soundData->buffer);

		// ���ʂ�ݒ�
		m_sourceVoice->SetVolume(volume);

		// �T�E���h���Đ�
		m_sourceVoice->Start();

		// �T�E���h�Ǘ��N���X���쐬���āA�Đ������ǂ����𔻒�A�Ǘ����������悢�̂ł�
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

			// ��Ԃ��擾���ĕ\��
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
		UINT64 playedSampleNum = state.SamplesPlayed - m_restartSamplesPlayed;
		DWORD sampleRate = soundData->waveData.wfx->nSamplesPerSec;

		// ���[�v�Đ����̍Đ��ʒu���v�Z
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

		XAUDIO2_VOICE_STATE state;
		m_sourceVoice->GetState(&state);

		// �Ȃ�i�߂鎞�͒l���}�C�i�X����
		// �Ȃ�߂����͒l���v���X����
		INT64 newRestartSamplesPlayed = m_restartSamplesPlayed - displacement;
		
		// �I�[�o�[�t���[��h��
		if (state.SamplesPlayed > newRestartSamplesPlayed)
		{
			m_restartSamplesPlayed = newRestartSamplesPlayed;
		}
	}
}