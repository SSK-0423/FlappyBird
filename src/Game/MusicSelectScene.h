#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	/// <summary>
	/// �ȑI���V�[��
	/// </summary>
	class MusicSelectScene : public Framework::Scene
	{
	public:
		MusicSelectScene();
		~MusicSelectScene() = default;

		void Init() override;
		void Final() override;
	};
}