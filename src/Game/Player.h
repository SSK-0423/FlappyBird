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

		bool IsDead();
	private:
		std::shared_ptr<Framework::Sprite> m_jumpSprite;
		std::shared_ptr<Framework::Sprite> m_fallSprite;

		void ChangeSprite();

		float m_jumpVelocity;
		void Move(float deltaTime);

		// Ž€–S‚µ‚½‚©‚Ç‚¤‚©
		bool m_isAlive;
		void OnDead();
	};
}