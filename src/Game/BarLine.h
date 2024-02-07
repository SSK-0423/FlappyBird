#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	enum class BARLINETYPE
	{
		WHOLE,
		QUARTER,
		EIGHTH,
	};

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

		bool CanPlaySE();
		void SetCanPlaySE(bool canPlaySE);

		void SetBarLineType(BARLINETYPE type);

		static void SetCurrentPlayTime(float currentPlayTime);
		static void SetJudgeLineX(float judgeLineX);
	private:
		Framework::Transform2D* m_transform = nullptr;
		float m_timing = 0.0f;
		bool m_canPlaySE = true;

		void UpdatePosition();

		static float m_judgeLineX;
		static float m_currentPlayTime;
	};
}