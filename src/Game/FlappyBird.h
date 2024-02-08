#pragma once
#include "Framework\GameImpl.h"

namespace FlappyBird
{
	enum SPRITE_LAYER
	{
		UI = 0,
		GAMEOBJECT = 1,
		OBSTACLE = 2,
		BARLINE = 3,
		BACKGROUND = 4,
	};

	class FlappyBird : public Framework::GameImpl
	{
	public:
		FlappyBird() = default;
		~FlappyBird() = default;

		void Init() override;
		void Final() override;
	};
}