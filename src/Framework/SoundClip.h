#pragma once

#include <xaudio2.h>
#include "WAVFileReader.h"
#include "IComponent.h"

namespace Framework
{
	class SoundClip : public IComponent
	{
	public:
		SoundClip(Object* owner);
		~SoundClip();
	private:
		// �T�E���h�f�[�^
		IXAudio2SourceVoice* m_sourceVoice;

	public:
		// �T�E���h�f�[�^�̐ݒ�
		Utility::RESULT LoadWavSound(const wchar_t* filename);
		std::unique_ptr<uint8_t[]> wavData;
		DirectX::WAVData m_wavData;

		void Update(float deltaTime) override;
		void Draw() override;

		void PlayOneShot(bool wait = false);
		void PlayLoop();
	};
}