#pragma once
#include "Framework\GameImpl.h"

namespace FlappyBird
{
	/// <summary>
	/// このゲームにおけるスプライトのレイヤー
	/// </summary>
	enum SPRITE_LAYER
	{
		UI = 0,
		GAMEOBJECT = 1,
		OBSTACLE = 2,
		BARLINE = 3,
		BACKGROUND = 4,
	};

	/// <summary>
	/// ゲームの実装クラス
	/// </summary>
	class FlappyBird : public Framework::GameImpl
	{
	public:
		FlappyBird() = default;
		~FlappyBird() = default;

		void Init() override;
		void Final() override;
	};
}