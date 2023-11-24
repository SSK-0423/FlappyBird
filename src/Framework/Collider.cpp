#include "pch.h"
#include "Collider.h"
#include "CollisionSystem.h"

namespace Framework
{
	Collider::Collider(Object* owner) : IComponent(owner)
	{
		CollisionSystem::AddCollider(this);
	}
}
