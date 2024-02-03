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
		// �ꎞ��~���łȂ��ꍇ�͐V�K��SourceVoice���擾����
		if (m_isPaused == false)
		{
			m_sourceVoice = SoundManager::Play(m_soundname);
		}

		if (m_sourceVoice == nullptr)
		{
			return;
		}
		m_sourceVoice->SetVolume(volume);

		// �Đ������ǂ����𔻒肷��K�v������
		// �����Ƀf���[�g����Ă͍���̂ŁA��x�Đ����I���܂ő҂�
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

		//Utility::DebugLog("�Đ��I��\n");
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
		UINT64 playedSampleNum = state.SamplesPlayed;
		DWORD sampleRate = soundData->waveData.wfx->nSamplesPerSec;

		return (float)playedSampleNum / sampleRate;
	}
}