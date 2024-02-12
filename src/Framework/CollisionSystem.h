#pragma once

namespace Framework
{
	class Collider;

	/// <summary>
	/// �Փ˔�����s���N���X
	/// </summary>
	class CollisionSystem
	{
	public:
		static void Update(float deltaTime);

		/// <summary>
		/// �R���C�_�[���Փ˔���Ώۂɒǉ�����
		/// </summary>
		/// <param name="collider">�R���C�_�[</param>
		static void AddCollider(Collider* collider);

		/// <summary>
		/// �R���C�_�[���Փ˔���Ώۂ���폜����
		/// </summary>
		/// <param name="collider">�R���C�_�[</param>
		static void RemoveCollider(Collider* collider);

		/// <summary>
		/// �Փ˔���Ώۂ�S�č폜����
		/// Collider�R���|�[�l���g��Object�̔j�����ɍ폜����邽�߁A
		/// ���̊֐����ł�delete�͍s��Ȃ�
		/// </summary>
		static void Reset();

	private:
		CollisionSystem() = default;
		~CollisionSystem();

		/// <summary>
		/// �Փ˔�����s��
		/// </summary>
		/// <param name="collider1">�R���C�_�[1</param>
		/// <param name="collider2">�R���C�_�[2</param>
		/// <returns>true�F�Փ˂��Ă��� false�F�Փ˂��Ă��Ȃ�</returns>
		static bool CollisionDetection(const Collider& collider1, const Collider& collider2);

		/// <summary>
		/// ��`���m�̏Փ˔�����s��
		/// </summary>
		/// <param name="collider1">��`1</param>
		/// <param name="collider2">��`2</param>
		/// <returns>true�F�Փ˂��Ă��� false�F�Փ˂��Ă��Ȃ�</returns>
		static bool RectRect(const Collider& collider1, const Collider& collider2);

		// �K�v������Α��̔�����ǉ�

		static std::list<Collider*> m_colliders;	// �Փ˔���ΏۂƂȂ�R���C�_�[�̃��X�g
	};
}