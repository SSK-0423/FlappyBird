#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class TitleSceneUI : public Framework::IComponent
	{
	public:
		TitleSceneUI(Framework::Object* owner);
		~TitleSceneUI() = default;

		void Update(float deltaTime) override;
		void Draw() override;

	private:
		std::vector<Framework::UIObject*> m_gameModeButtons;
		void ShowGameModeButtons(Framework::NotificationEvent e);
	};
}