#pragma once
#include "Framework/Scene.h"

namespace FlappyBird
{
	/// <summary>
	/// �Q�[���V�[��
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


