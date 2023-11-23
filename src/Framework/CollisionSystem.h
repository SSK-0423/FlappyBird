#pragma once
#include "Collider.h"

namespace Framework
{
	class CollisionSystem
	{
	public:
		static void Update(float deltaTime);
		static void AddCollider(std::shared_ptr<Collider>& collider);
		static void Reset();

	private:
		CollisionSystem() = default;
		~CollisionSystem();

		// �Փ˔���
		static bool CollisionDetection(const Collider& collider1, const Collider& collider2);

		// ��`���m�̔���
		static bool RectRect(const Collider& collider1, const Collider& collider2);

		// �K�v������Α��̔�����ǉ�

		static std::list<std::shared_ptr<Collider>> m_colliders;
	};
}