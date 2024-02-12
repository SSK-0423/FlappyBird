#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	/// <summary>
	/// ゲームオーバー時のUIを管理するコンポーネント
	/// </summary>
	class GameOverUI : public Framework::IComponent
	{
	public:
		GameOverUI(std::shared_ptr<Framework::Object> owner);
		~GameOverUI() = default;

		void Update(float deltaTime) override;
		void Draw() override;
	};
}