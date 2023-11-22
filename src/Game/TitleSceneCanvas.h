#pragma once
#include "Framework/Canvas.h"

namespace FlappyBird
{
	class TitleSceneCanvas : public Framework::Canvas
	{
	public:
		TitleSceneCanvas();
		~TitleSceneCanvas();

		void Init() override;
		void Final() override;
	};
}