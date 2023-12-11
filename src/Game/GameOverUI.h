#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class GameOverUI : public Framework::IComponent
	{
	public:
		GameOverUI(Framework::Object* owner);
		~GameOverUI() = default;

		void Update(float deltaTime) override;
		void Draw() override;
	};
}