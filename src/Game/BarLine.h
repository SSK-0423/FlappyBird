#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class BarLine : public Framework::IComponent
	{
	public:
		BarLine(Framework::Object* owner);
		~BarLine();

		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		void SetTiming(float timing);
		float GetTiming();

		void SetCurrentPlayTime(float currentPlayTime);

		bool CanPlaySE();
		void SetCanPlaySE(bool canPlaySE);

	private:
		Framework::Transform2D* m_transform = nullptr;
		float m_timing = 0.0f;
		float m_currentPlayTime = 0.0f;
		bool m_canPlaySE = true;
	};
}