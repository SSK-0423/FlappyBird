#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class MusicPlayer : public Framework::IComponent
	{
	public:
		MusicPlayer(std::shared_ptr<Framework::Object> owner);
		~MusicPlayer();

		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		void Load(const std::string& musicPath);
		void Play(float startTime = 0.f);
		void Stop();

		float GetMusicLength();
		float GetCurrentPlayTimeMs();
		void Seek(float timeSec);
		bool IsPlaying();

	private:
		Framework::SoundClip* m_music;
		wchar_t m_musicPath[256];
		bool m_isPlaying;
	};
}