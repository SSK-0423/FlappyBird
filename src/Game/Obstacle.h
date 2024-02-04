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
		float GetTiming();

		void SetPosY(float posY);
		void SetMaterialColor(const DirectX::XMFLOAT4& color);

		bool CanPlaySE();
		void SetCanPlaySE(bool canPlaySE);

		static void SetJudgeLineX(float judgeLineX);
		static void SetCurrentPlayTime(float currentPlayTime);

	private:
		float m_moveSpeedX = 0.0f;
		float m_moveSpeedY = 0.0f;

		float m_timing = 0.0f;
		float m_posY = 0.0f;

		bool m_canPlaySE = true;

		Framework::Transform2D* m_overObstacleTransform = nullptr;
		Framework::Transform2D* m_underObstacleTransform = nullptr;

		bool IsOutsideViewport();
		void UpdatePosition();
		void Reset();

		static const int SPACE;
		static float m_judgeLineX;
		static float m_currentPlayTime;
	};
}