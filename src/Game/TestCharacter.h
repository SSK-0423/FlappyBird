#pragma once

#include "Framework/IComponent.h"
#include "Framework/Collider.h"

using namespace Framework;

namespace FlappyBird
{
	class TestCharacter : public IComponent
	{
	public:
		TestCharacter(Object* owner);
		~TestCharacter();

		void Update(float deltaTime) override;
		void Draw() override;
		void OnCollision(Collider* other);
	};
}