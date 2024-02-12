#pragma once
#include "Framework\GameImpl.h"

namespace FlappyBird
{
	/// <summary>
	/// ���̃Q�[���ɂ�����X�v���C�g�̃��C���[
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
	/// �Q�[���̎����N���X
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