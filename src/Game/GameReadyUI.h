#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class GameReadyUI : public Framework::IComponent
	{
	public:
		GameReadyUI(Framework::Object* owner);
		~GameReadyUI() = default;

		void Update(float deltaTime) override;
		void Draw() override;
	};
}