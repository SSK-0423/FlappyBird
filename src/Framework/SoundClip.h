#pragma once

namespace Framework
{
	class SoundClip
	{
	public:
		SoundClip() = default;
		~SoundClip() = default;
	private:
		// �T�E���h�f�[�^

	public:
		void PlayOneShot();
		void PlayLoop();
	};
}