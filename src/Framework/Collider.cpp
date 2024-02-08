#include "pch.h"
#include "Collider.h"
#include "CollisionSystem.h"

namespace Framework
{
	Collider::Collider(std::shared_ptr<Object> owner) : IComponent(owner)
	{
		CollisionSystem::AddCollider(this);
	}
	Collider::~Collider()
	{
		CollisionSystem::RemoveCollider(this);
	}
	void Collider::SetOnCollisionCallBack(std::function<void(Collider*)> func)
	{
		m_onCollisionCallBack = func;
	}
	void Collider::OnCollision(Collider* other)
	{
		if (m_onCollisionCallBack != nullptr)
		{
			m_onCollisionCallBack(other);
		}
	}
}
