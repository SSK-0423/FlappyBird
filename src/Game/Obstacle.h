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
		void SetPosY(float posY);
		void SetMaterialColor(const DirectX::XMFLOAT4& color);

		static void SetJudgeLineX(float judgeLineX);
		static void SetCurrentPlayTime(float currentPlayTime);

	private:
		float m_moveSpeedX = 0.0f;
		float m_moveSpeedY = 0.0f;

		float m_timing = 0.0f;
		float m_posY = 0.0f;

		std::vector<Framework::Transform2D*> m_transforms;

		static const int SPACE;
		static float m_judgeLineX;
		static float m_currentPlayTime;
	};
}