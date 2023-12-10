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

		// 死亡フラグをオンにする
		void OnCollision(Framework::Collider* other);

	private:
		float m_jumpVelocity;
		
		void Move(float deltaTime);
		
		// 死亡したかどうか
		bool m_isAlive;
		void OnDead();
	};
}