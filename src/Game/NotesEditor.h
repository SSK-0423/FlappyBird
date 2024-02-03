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
		void LoadMusic(const std::string& musicPath);
		void StartEdit(const FumenData& data);

		void PutNotes();
		void DeleteNotes();

		float CalcNotesTiming(float mouseX, float viewportWidth);

		BarManager* m_barManager = nullptr;
		LaneManager* m_laneManager = nullptr;
		NotesManager* m_notesManager = nullptr;
		MusicPlayer* m_musicPlayer = nullptr;
	};
}