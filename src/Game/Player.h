#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class GameMaster;

	class Player : public Framework::IComponent
	{
	public:
		Player(Framework::Object* owner);
		~Player() = default;

		void Update(float deltaTime) override;
		void Draw() override;

		void OnCollision(Framework::Collider* other);

		bool IsDead();
	private:
		GameMaster* m_gameMaster;

		std::shared_ptr<Framework::Sprite> m_jumpSprite;
		std::shared_ptr<Framework::Sprite> m_fallSprite;
		void ChangeSprite();

		float m_jumpVelocity;
		void Move(float deltaTime);
		void Jump();
		void LimitPosition();

		float m_elapsedTime;
		float m_gameReadyAnimationInterval;
		void GameReadyAnimation(float deltaTime);
		void GameOverAnimation(float deltaTime);

		// Ž€–S‚µ‚½‚©‚Ç‚¤‚©
		bool m_isAlive;
		void OnDead();
	};
}