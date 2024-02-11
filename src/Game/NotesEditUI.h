#pragma once

#include "../Framework/GameFramework.h"
#include "FumenData.h"

namespace FlappyBird
{
	class NotesEditUI : public Framework::IComponent
	{
	public:
		NotesEditUI(std::shared_ptr<Framework::Object> owner);
		~NotesEditUI() = default;

		void Update(float deltaTime) override;
		void Draw() override;

		// 各ボタン押下時のイベント
		Framework::Subject<const std::string&, FumenData&> OnSave;
		Framework::Subject<const std::string&, FumenData&> OnLoad;
		Framework::Subject<const FumenData&> OnEditStart;
		Framework::Subject<Framework::NotificationEvent> OnPlay;
		Framework::Subject<Framework::NotificationEvent> OnStop;
		Framework::Subject<Framework::NotificationEvent> OnRestart;

	private:
		static const ImVec2 WINDOW_POS;
		static const ImVec2 WINDOW_SIZE;

		FumenData m_fumenData;

		void DrawFumenData();
		void DrawSaveLoadButton();
		void DrawPlayStopRestartButton();
		void DrawEditStartButton();

		static std::string m_selectedMusicPath;
	};
}