#pragma once

#include "../Framework/GameFramework.h"
#include "FumenData.h"

namespace FlappyBird
{
	class NotesEditUI : public Framework::IComponent
	{
	public:
		NotesEditUI(Framework::Object* owner);
		~NotesEditUI() = default;

		void Update(float deltaTime) override;
		void Draw() override;

		// 各ボタン押下時のイベント
		Framework::Subject<const std::string&, const FumenData&> OnSave;
		Framework::Subject<const std::string&, FumenData&> OnLoad;
		Framework::Subject<const std::string&> OnLoadMusic;
		Framework::Subject<const FumenData&> OnEditStart;
		Framework::Subject<Framework::NotificationEvent> OnPlay;
		Framework::Subject<Framework::NotificationEvent> OnStop;

	private:
		FumenData m_fumenData;

		void DrawFumenData();
		void DrawSaveLoadButton();
		void DrawPlayStopButton();
		void DrawEditStartButton();

		static std::string m_selectedMusicPath;
	};
}