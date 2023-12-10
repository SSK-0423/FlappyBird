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

		// Ž€–Sƒtƒ‰ƒO‚ðƒIƒ“‚É‚·‚é
		void OnCollision(Framework::Collider* other);

	private:
		float m_jumpVelocity;
		
		void Move(float deltaTime);
		
		// Ž€–S‚µ‚½‚©‚Ç‚¤‚©
		bool m_isAlive;
		void OnDead();
	};
}