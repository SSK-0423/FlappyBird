#pragma once
#include "../Framework/Scene.h"

namespace FlappyBird
{
	/// <summary>
	/// ノーツエディットシーン
	/// </summary>
	class NotesEditScene : public Framework::Scene
	{
	public:
		NotesEditScene() = default;
		~NotesEditScene() = default;

		void Init() override;
		void Final() override;
	};
}