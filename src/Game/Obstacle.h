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

	private:
		float m_moveSpeedX = 0.0f;
		float m_moveSpeedY = 0.0f;
	};
}