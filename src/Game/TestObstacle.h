#pragma once

#include "Framework/IComponent.h"

using namespace Framework;

namespace FlappyBird
{
	class TestObstacle : public IComponent
	{
	public:
		TestObstacle(Object* owner);
		~TestObstacle();

		void Update(float deltaTime) override;
		void Draw() override;
	};
}