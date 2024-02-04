#pragma once
#include "Framework/GameFramework.h"

namespace FlappyBird
{
	struct Note
	{
		float timing;
		float posY;

		Note() : timing(0.f), posY(0.f) {}
		Note(float timing, float posY) : timing(timing), posY(posY) {}
	};

	class NotesManager : public Framework::IComponent
	{
	public:
		NotesManager(Framework::Object* owner);
		~NotesManager();

		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		void CreateNotes(Note data);
		void DeleteNotes(Note data);

	private:
		std::vector<Note> m_notes;
	};
}