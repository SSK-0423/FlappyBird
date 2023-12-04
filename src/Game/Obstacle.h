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

		void OnCollision(Framework::Collider* other);
	};
}