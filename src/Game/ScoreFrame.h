#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class ScoreFrame : public Framework::IComponent
	{
	public:
		ScoreFrame(Framework::Object* owner);
		~ScoreFrame() = default;

		void Update(float deltaTime) override;
		void Draw() override;
	};
}