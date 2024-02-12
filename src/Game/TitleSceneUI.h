#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	/// <summary>
	/// タイトルシーンのUIを管理するコンポーネント
	/// </summary>
	class TitleSceneUI : public Framework::IComponent
	{
	public:
		TitleSceneUI(std::shared_ptr<Framework::Object> owner);
		~TitleSceneUI() = default;

		void Update(float deltaTime) override;
		void Draw() override;

	private:
		std::vector<std::shared_ptr<Framework::UIObject>> m_gameModeButtons;	// ゲームモード選択ボタン

		/// <summary>
		/// ゲームモード選択ボタンを表示する
		/// </summary>
		/// <param name="e"></param>
		void ShowGameModeButtons(Framework::NotificationEvent e);
	};
}