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
		Collider(Object* owner);
		virtual ~Collider() = default;

		virtual void Update(float deltaTime) = 0;
		virtual void Draw() = 0;

		virtual void* GetColliderShape() const = 0;

		std::string GetTag() { return m_tag; };

		COLLIDER_TYPE GetColliderType() const { return m_colliderType; };

		// •K—v‚È‚ç‚ÎOnCollisionEnter, OnCollisionStay, OnCollisionExit‚ðŽÀ‘•‚·‚é
		void SetOnCollisionCallBack(std::function<void(Collider*)> func);
		void OnCollision(Collider* other);

	protected:
		std::string m_tag = "Default";
		COLLIDER_TYPE m_colliderType = COLLIDER_TYPE::NONE;
		std::function<void(Collider*)> m_onCollisionCallBack = nullptr;
	};
}