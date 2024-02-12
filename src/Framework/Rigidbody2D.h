#pragma once

#include "IComponent.h"

#include <DirectXMath.h>

namespace Framework
{
	/// <summary>
	/// �͂̎��
	/// </summary>
	enum class FORCE_MODE
	{
		ACCELERATION,
		VELOCITY,
		FORCE,
	};

	/// <summary>
	/// 2D�̍��̂̕������Z���s���R���|�[�l���g
	/// </summary>
	class Rigidbody2D : public IComponent
	{
	public:
		Rigidbody2D(std::shared_ptr<Object> owner);
		~Rigidbody2D();

		void Update(float deltaTime) override;
		void FixedUpdate(float interval) override;
		void Draw() override;
		void DrawInspector() override;

		/// <summary>
		/// ���̃R���|�[�l���g���A�^�b�`����Ă���I�u�W�F�N�g�ɗ͂�������
		/// </summary>
		/// <param name="force">�͂̃x�N�g����</param>
		/// <param name="mode">�͂̎��</param>
		void AddForce(DirectX::XMFLOAT2 force, FORCE_MODE mode = FORCE_MODE::FORCE);

		float mass = 1.f;		                        // ����
		float gravity = 9.8f;	                        // �d�͉����x
		float gravityScale = 1.f;	                    // �d�͂̃X�P�[��
		bool useGravity = true;	                        // �d�͂��g�p���邩�ǂ���

		DirectX::XMFLOAT2 velocity = { 0.f, 0.f };	    // ���x
		DirectX::XMFLOAT2 acceleration = { 0.f, 0.f };	// �����x
	};
}