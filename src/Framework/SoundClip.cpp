#include "SoundClip.h"
#include "SoundManager.h"

#include "Editor.h"

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
}