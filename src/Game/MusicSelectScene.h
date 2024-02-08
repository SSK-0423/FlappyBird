#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class MusicSelectScene : public Framework::Scene
	{
	public:
		MusicSelectScene();
		~MusicSelectScene() = default;

		void Init() override;
		void Final() override;
	};
}