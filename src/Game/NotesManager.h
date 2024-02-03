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
	};

	class NotesManager : public Framework::IComponent
	{
	public:
		NotesManager(Framework::Object* owner);
		~NotesManager();

		void Update(float deltaTime) override;
		void Draw() override;

		void CreateNotes(float x, float y, Note data);
		void DeleteNotes(Note data);

	private:
		std::vector<Note> m_notes;
	};
}