#pragma once
#include "../Framework/Scene.h"

namespace FlappyBird
{
	enum class TITLE_SCENE_LAYER
	{
		UI,
		BACKGROUND,
	};

	class TitleScene : public Framework::Scene
	{
	public:
		TitleScene() = default;
		~TitleScene() = default;

		void Init() override;
		void Final() override;
	};
}