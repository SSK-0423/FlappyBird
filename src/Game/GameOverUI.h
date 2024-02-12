#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	/// <summary>
	/// �Q�[���I�[�o�[����UI���Ǘ�����R���|�[�l���g
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