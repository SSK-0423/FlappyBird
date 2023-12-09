#pragma once
#include "SoundClip.h"

namespace Framework
{
	class SoundManager
	{
	private:
		SoundManager() = default;
		~SoundManager() = default;

	public:
		static SoundManager& Instance()
		{
			static SoundManager inst;
			return inst;
		}

		void PlayOneShot(const char* name);

		void PlayLoop(const char* name);

	private:
		std::vector<SoundClip> m_SoundClips;
	};
}