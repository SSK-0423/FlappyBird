#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	/// <summary>
	/// ゲーム全体の進行を管理するクラス
	/// </summary>
	class GameMaster : public Framework::IComponent
	{
	public:
		GameMaster(Framework::Object* owner);
		~GameMaster();

		void Update(float deltaTime) override;
		void Draw() override;
	};
}