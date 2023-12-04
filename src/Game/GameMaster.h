#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	/// <summary>
	/// �Q�[���S�̂̐i�s���Ǘ�����N���X
	/// </summary>
	class GameMaster : public Framework::IComponent
	{
	public:
		GameMaster(Framework::Object* owner);
		~GameMaster();

		void Update(float deltaTime) override;
		void Draw() override;
	};
}