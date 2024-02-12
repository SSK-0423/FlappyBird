#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	/// <summary>
	/// �ȃf�[�^�\����UI�R���|�[�l���g
	/// </summary>
	class MusicDataFrame : public Framework::IComponent
	{
	public:
		MusicDataFrame(std::shared_ptr<Framework::Object> owner);
		~MusicDataFrame() = default;

		void Update(float deltaTime) override;
		void Draw() override;

		/// <summary>
		/// �e�L�X�g�̈ʒu���t���[���ɍ��킹��
		/// </summary>
		void FitTextPosition();
	};
}