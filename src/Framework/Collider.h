#pragma once
#include "IComponent.h"

namespace Framework
{
	enum class COLLIDER_TYPE
	{
		RECT,
		CIRCLE,
		NONE,
	};


	class Collider : public IComponent
	{
	public:
		Collider(Object* owner) : IComponent(owner) {};
		virtual ~Collider() = default;

		virtual void Update(float deltaTime) = 0;
		virtual void Draw() = 0;

		virtual void* GetColliderShape() const = 0;

		COLLIDER_TYPE GetColliderType() const { return m_colliderType; };

	protected:
		COLLIDER_TYPE m_colliderType = COLLIDER_TYPE::NONE;
	};
}