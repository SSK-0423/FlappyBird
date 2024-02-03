#pragma once
#include "Framework/GameFramework.h"

namespace FlappyBird
{
	struct Note
	{
		float timing;
		int lane;
		int barNum;
		int lineNum;

		Note() : timing(0.0f), lane(0), barNum(0), lineNum(0) {}
		Note(float timing) : timing(timing), lane(0), barNum(0), lineNum(0) {}
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