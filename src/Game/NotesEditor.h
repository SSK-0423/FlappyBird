#pragma once

#include "Framework/GameFramework.h"
#include "FumenData.h"

namespace FlappyBird
{
	class BarManager;
	class LaneManager;
	class NotesManager;
	class MusicPlayer;

	class NotesEditor : public Framework::IComponent
	{
	public:
		NotesEditor(Framework::Object* owner);
		~NotesEditor();

		void Init();
		void Update(float deltaTime) override;
		void Draw() override;

	private:
		void SaveFumen(const std::string& savePath, const FumenData& data);
		void LoadFumen(const std::string& loadPath, FumenData& data);
		void Play();
		void Stop();

		BarManager* m_barManager;
		LaneManager* m_laneManager;
		NotesManager* m_notesManager;
		MusicPlayer* m_musicPlayer;
	};
}