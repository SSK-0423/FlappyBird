#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class MusicPlayer : public Framework::IComponent
	{
	public:
		MusicPlayer(Framework::Object* owner);
		~MusicPlayer();

		void Update(float deltaTime) override;
		void Draw() override;

		void Load(const char* musicPath);
		void Play();
		void Stop();

	private:
		Framework::SoundClip* m_music;
		wchar_t m_musicPath[256];
		bool m_isPlaying;
	};
}