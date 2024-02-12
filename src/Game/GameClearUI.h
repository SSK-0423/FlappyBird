#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	/// <summary>
	/// ゲームクリア時のUIを管理するコンポーネント
	/// </summary>
	class GameClearUI : public Framework::IComponent
	{
	public:
		GameClearUI(std::shared_ptr<Framework::Object> owner);
		~GameClearUI() = default;

		void Update(float deltaTime) override;
		void Draw() override;
	};
}