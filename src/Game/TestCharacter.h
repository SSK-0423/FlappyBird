#pragma once

#include "Framework/IComponent.h"

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
	};
}