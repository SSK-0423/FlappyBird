#pragma once
#include "Framework\GameImpl.h"

namespace FlappyBird
{
	enum SPRITE_LAYER
	{
		UI = 0,
		GAMEOBJECT = 1,
		BACKGROUND = 2,
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