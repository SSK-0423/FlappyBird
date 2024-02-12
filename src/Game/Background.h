#pragma once

#include "Framework/IComponent.h"

namespace FlappyBird
{
	/// <summary>
	/// 背景を描画するコンポーネント
	/// </summary>
	class Background : public Framework::IComponent
	{
	public:
		Background(std::shared_ptr<Framework::Object> owner);
		~Background();

		void Update(float deltaTime) override;
		void Draw() override;
	};
}