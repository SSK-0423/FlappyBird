#pragma once
#include "Framework/Scene.h"

namespace FlappyBird
{
	enum class GAME_SCENE_LAYER
	{
		UI = 0,
		GAMEOBJECT = 1,
		BACKGROUND = 2,
	};

	class GameScene : public Framework::Scene
	{
	public:
		GameScene() = default;
		~GameScene() = default;

		void Init() override;
		void Final() override;
	};
}


