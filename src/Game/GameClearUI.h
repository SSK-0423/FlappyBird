#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class GameClearUI : public Framework::IComponent
	{
	public:
		GameClearUI(std::shared_ptr<Framework::Object> owner);
		~GameClearUI() = default;

		void Update(float deltaTime) override;
		void Draw() override;
	};
}