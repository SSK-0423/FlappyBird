#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class Obstacle : public Framework::IComponent
	{
	public:
		Obstacle(Framework::Object* owner);
		~Obstacle();

		void Update(float deltaTime) override;
		void Draw() override;

		void SetMoveSpeed(float moveSpeedX, float moveSpeedY);

		void SetTiming(float timing);

		static void SetJudgeLineX(float judgeLineX);
		static void SetCurrentPlayTime(float currentPlayTime);

	private:
		float m_moveSpeedX = 0.0f;
		float m_moveSpeedY = 0.0f;

		float m_timing = 0.0f;

		Framework::Transform2D* m_transform = nullptr;

		static float m_judgeLineX;
		static float m_currentPlayTime;
	};
}