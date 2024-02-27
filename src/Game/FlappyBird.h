#pragma once
#include "Framework\GameImpl.h"

namespace FlappyBird
{
	/// <summary>
	/// このゲームにおけるスプライトのレイヤー
	/// </summary>
	enum SPRITE_LAYER
	{
		UI,
		GAMEOBJECT,
		JUMP_POINT,
		OBSTACLE,
		BARLINE,
		BACKGROUND,
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