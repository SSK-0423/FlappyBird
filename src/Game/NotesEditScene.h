#pragma once
#include "../Framework/Scene.h"

namespace FlappyBird
{
	enum class NOTESEDIT_SCENE_LAYER
	{
		UI,
		BACKGROUND,
	};


	class NotesEditScene : public Framework::Scene
	{
	public:
		NotesEditScene() = default;
		~NotesEditScene() = default;

		void Init() override;
		void Final() override;
	};
}