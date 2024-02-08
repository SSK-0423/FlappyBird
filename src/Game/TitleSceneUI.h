#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class TitleSceneUI : public Framework::IComponent
	{
	public:
		TitleSceneUI(std::shared_ptr<Framework::Object> owner);
		~TitleSceneUI() = default;

		void Update(float deltaTime) override;
		void Draw() override;

	private:
		std::vector<std::shared_ptr<Framework::UIObject>> m_gameModeButtons;
		void ShowGameModeButtons(Framework::NotificationEvent e);
	};
}