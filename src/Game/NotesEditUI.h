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
		Framework::Subject<Framework::NotificationEvent> OnPlay;
		Framework::Subject<Framework::NotificationEvent> OnStop;
		Framework::Subject<const std::string&> OnLoadMusic;

	private:
		FumenData m_fumenData;

		void DrawFumenData();
		void DrawSaveLoadButton();
		void DrawPlayStopButton();

		static std::string m_selectedMusicPath;
	};
}