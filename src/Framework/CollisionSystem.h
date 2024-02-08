#pragma once

namespace Framework
{
	class Collider;

	class CollisionSystem
	{
	public:
		static void Update(float deltaTime);
		static void AddCollider(Collider* collider);
		static void RemoveCollider(Collider* collider);
		static void Reset();

	private:
		CollisionSystem() = default;
		~CollisionSystem();

		// �Փ˔���
		static bool CollisionDetection(const Collider& collider1, const Collider& collider2);

		// ��`���m�̔���
		static bool RectRect(const Collider& collider1, const Collider& collider2);

		// �K�v������Α��̔�����ǉ�

		static std::list<Collider*> m_colliders;
	};
}