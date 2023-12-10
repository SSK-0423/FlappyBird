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
	}
	void SoundClip::Stop()
	{
		if (m_sourceVoice != nullptr)
		{
			m_sourceVoice->Stop();
		}
	}
}