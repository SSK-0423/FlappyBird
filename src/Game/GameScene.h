#pragma once
#include "Framework/Scene.h"

namespace FlappyBird
{
	/// <summary>
	/// ƒQ[ƒ€ƒV[ƒ“
	/// </summary>
	class GameScene : public Framework::Scene
	{
	public:
		GameScene() = default;
		~GameScene() = default;

		void Init() override;
		void Final() override;
	};
}


