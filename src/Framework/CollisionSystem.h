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

		// Õ“Ë”»’è
		static bool CollisionDetection(const Collider& collider1, const Collider& collider2);

		// ‹éŒ`“¯m‚Ì”»’è
		static bool RectRect(const Collider& collider1, const Collider& collider2);

		// •K—v‚ª‚ ‚ê‚Î‘¼‚Ì”»’è‚à’Ç‰Á

		static std::list<std::shared_ptr<Collider>> m_colliders;
	};
}