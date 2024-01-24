#pragma once

#include "../Framework/GameFramework.h"

namespace FlappyBird
{
	class NotesEditUI : public Framework::IComponent
	{
	public:
		NotesEditUI(Framework::Object* owner);
		~NotesEditUI() = default;

		void Update(float deltaTime) override;
		void Draw() override;
	};
}