#pragma once
#include "IComponent.h"

namespace Framework
{
	/// <summary>
	/// �R���C�_�[�`��̎��
	/// </summary>
	enum class COLLIDER_TYPE
	{
		RECT,
		CIRCLE,
		NONE,
	};

	/// <summary>
	/// �R���C�_�[�R���|�[�l���g�̊��N���X
	/// </summary>
	class Collider : public IComponent
	{
	public:
		Collider(std::shared_ptr<Object> owner);
		virtual ~Collider();

		virtual void Update(float deltaTime) = 0;
		virtual void Draw() = 0;
		virtual void DrawInspector() {};

		/// <summary>
		/// �R���C�_�[�̌`����擾����
		/// </summary>
		/// <returns>void*�ɕϊ����ꂽ�`��f�[�^</returns>
		virtual void* GetColliderShape() const = 0;

		/// <summary>
		/// �R���C�_�[�̌`��^�C�v���擾����
		/// ���̐��l�����ɁAGetColliderShape�Ŏ擾�����f�[�^���L���X�g����
		/// </summary>
		/// <returns>�`��^�C�v</returns>
		COLLIDER_TYPE GetColliderType() const { return m_colliderType; };

		// �K�v�Ȃ��OnCollisionEnter, OnCollisionStay, OnCollisionExit����������

		/// <summary>
		/// �Փˎ��ɌĂ΂��R�[���o�b�N�֐���ݒ肷��
		/// </summary>
		/// <param name="func"></param>
		void SetOnCollisionCallBack(std::function<void(Collider*)> func);

		/// <summary>
		/// �Փˎ��ɌĂ΂��R�[���o�b�N�֐�
		/// ���̊֐�����SetOnCollisionCallBack�Őݒ肵���֐����Ă΂��
		/// </summary>
		/// <param name="other">�Փ˂����R���C�_�[</param>
		void OnCollision(Collider* other);

	protected:
		COLLIDER_TYPE m_colliderType = COLLIDER_TYPE::NONE;
		std::function<void(Collider*)> m_onCollisionCallBack = nullptr;
	};
}