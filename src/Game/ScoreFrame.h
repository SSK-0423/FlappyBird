#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	/// <summary>
	/// スコアを描画するフレーム
	/// </summary>
	class ScoreFrame : public Framework::IComponent
	{
	public:
		ScoreFrame(std::shared_ptr<Framework::Object> owner);
		~ScoreFrame() = default;

		void Update(float deltaTime) override;
		void Draw() override;
	};
}