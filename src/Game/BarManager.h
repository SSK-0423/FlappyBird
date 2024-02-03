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

		void Update(float deltaTime) override;
		void Draw() override;

		void CreateBar(unsigned int barNum, float bpm, int beat);
	private:
		// ���ߐ��̈ꗗ
		std::vector<BarLine*> m_barLines;

		MusicPlayer* m_musicPlayer = nullptr;
	};
}