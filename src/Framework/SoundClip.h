#pragma once

namespace Framework
{
	class SoundClip
	{
	public:
		SoundClip() = default;
		~SoundClip() = default;
	private:
		// サウンドデータ

	public:
		void PlayOneShot();
		void PlayLoop();
	};
}