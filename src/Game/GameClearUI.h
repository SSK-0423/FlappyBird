#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	/// <summary>
	/// �Q�[���N���A����UI���Ǘ�����R���|�[�l���g
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