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
	private:
		FumenData m_fumenData;

		void DrawFumenData();
		void DrawSaveLoadButton();

		static std::string m_selectedMusicPath;
	};
}