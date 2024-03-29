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

		// 衝突判定
		static bool CollisionDetection(const Collider& collider1, const Collider& collider2);

		// 矩形同士の判定
		static bool RectRect(const Collider& collider1, const Collider& collider2);

		// 必要があれば他の判定も追加

		static std::list<Collider*> m_colliders;
	};
}