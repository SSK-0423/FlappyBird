#include "SoundClip.h"
#include "SoundManager.h"

namespace Framework
{
	SoundClip::SoundClip(Object* owner) : 
		IComponent(owner), m_soundname(nullptr), m_sourceVoice(nullptr)
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
	void SoundClip::Play(bool wait)
	{
		m_sourceVoice = SoundManager::Play(m_soundname);

		if (m_sourceVoice == nullptr)
		{
			return;
		}

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
	}
	void SoundClip::Stop()
	{
		if (m_sourceVoice != nullptr)
		{
			m_sourceVoice->Stop();
		}
	}
}