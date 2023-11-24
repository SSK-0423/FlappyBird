#pragma once
#include "Framework/Scene.h"

namespace FlappyBird
{
	class GameScene : public Framework::Scene
	{
	public:
		GameScene() = default;
		~GameScene() = default;

		void Init() override;
		void Final() override;
	};
}


