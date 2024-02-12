#pragma once
#include "Framework/GameFramework.h"

namespace FlappyBird
{
	/// <summary>
	/// PleaseClickText��_�ł����Ȃ���\������R���|�[�l���g
	/// </summary>
	class PleaseClickText : public Framework::IComponent
	{
	public:
		PleaseClickText(std::shared_ptr<Framework::Object> owner);
		~PleaseClickText() = default;

		void Update(float deltaTime) override;
		void Draw() override;

		Framework::Subject<Framework::NotificationEvent> OnClicked; // �N���b�N���ꂽ�Ƃ��ɒʒm����C�x���g

	private:
		float m_elapsedBlinkTime;		// �_�ŃA�j���[�V�����̌o�ߎ���
		float m_blinkAnimationInterval;	// �_�ł̊Ԋu

		float m_elapsedWaitTime;		// ���艹�̍Đ��I���܂ł̌o�ߎ���
		float m_waitSoundTime;			// ���艹�̍Đ�����

		bool m_isClicked;				// �N���b�N���ꂽ���ǂ���

		std::vector<DirectX::XMVECTORF32> m_colors;	// ���Ɠ����̐F
		size_t m_currentColorIndex;					// ���݂̐F�̃C���f�b�N�X

		/// <summary>
		/// �_�ŃA�j���[�V����
		/// </summary>
		/// <param name="deltaTime"></param>
		void BlinkAnimation(float deltaTime);
	};
}