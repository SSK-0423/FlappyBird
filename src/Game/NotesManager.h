#pragma once
#include "Framework/GameFramework.h"
#include "NoteData.h"

namespace FlappyBird
{
	class Obstacle;
	class MusicPlayer;

	class NotesManager : public Framework::IComponent
	{
	public:
		NotesManager(Framework::Object* owner);
		~NotesManager();

		void Start();
		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		void CreateNotes(NoteData data);
		void DeleteNotes(NoteData data);

		std::vector<NoteData>& GetNotes();
		void SetNotes(const std::vector<NoteData>& notes);
	private:
		std::vector<NoteData> m_notes;
		std::vector<Obstacle*> m_noteObstacles;

		MusicPlayer* m_musicPlayer = nullptr;

		float m_currentPlayTime = 0.0f;

		void UpdateCurrentPlayTime();
		void UpdateNoteActive();
	};
}