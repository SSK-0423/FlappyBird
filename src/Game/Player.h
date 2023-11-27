#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class Player : public Framework::IComponent
	{
	public:
		Player(Framework::Object* owner);
		~Player();

		void Update(float deltaTime) override;
		void Draw() override;

		void OnCollision(Framework::Collider* other);

	private:
		float m_jumpVelocity;
	};
}