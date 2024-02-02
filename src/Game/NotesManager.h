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
		~NotesManager() = default;

		void Update(float deltaTime) override;
		void Draw() override;

		static void CreateNotes(float x, float y, Note data);
		static void DeleteNotes(Note data) {};
	};
}