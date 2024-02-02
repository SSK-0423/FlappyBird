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

		Framework::Subject<FumenData> OnSave;
		Framework::Subject<FumenData> OnLoad;
		Framework::Subject<Framework::NotificationEvent> OnPlay;
		Framework::Subject<Framework::NotificationEvent> OnStop;

	private:
		FumenData m_fumenData;

		void DrawFumenData();
		void DrawSaveLoadButton();
		void DrawPlayStopButton();

		static std::string m_selectedMusicPath;
	};
}