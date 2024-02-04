#pragma once
#include "Framework/GameFramework.h"
#include "BarLine.h"

namespace FlappyBird
{
	class MusicPlayer;

	class BarManager : public Framework::IComponent
	{
	public:
		BarManager(Framework::Object* owner);
		~BarManager() = default;

		void Start();
		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		void CreateBar(unsigned int barNum, float bpm, int beat);

		float GetNearBarLineTiming(float timing);
	private:
		// ¬ßü‚Ìˆê——
		std::vector<BarLine*> m_barLines;

		MusicPlayer* m_musicPlayer = nullptr;

		void UpdateCurrentPlayTime();
		void UpdateBarLineActive();
	};
}