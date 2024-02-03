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

	public:
		// �T�E���h�f�[�^�̐ݒ�
		Utility::RESULT LoadWavSound(const wchar_t* filename, bool isLoop = false);

		void Update(float deltaTime) override;
		void Draw() override;

		/// <summary>
		/// �T�E���h�Đ�
		/// </summary>
		/// <param name="wait">���̍Đ����I������܂ő҂�</param>
		void Play(float volume = 0.1f, bool wait = false);

		void Stop(bool isPause = false);

	private:
		const wchar_t* m_soundname;
		IXAudio2SourceVoice* m_sourceVoice;
		bool m_isPaused;
		bool m_isPlaying;
	};
}