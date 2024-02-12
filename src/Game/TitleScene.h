#pragma once
#include "../Framework/Scene.h"

namespace FlappyBird
{
	/// <summary>
	/// タイトルシーンクラス
	/// </summary>
	class TitleScene : public Framework::Scene
	{
	public:
		TitleScene() = default;
		~TitleScene() = default;

		void Init() override;
		void Final() override;
	};
}