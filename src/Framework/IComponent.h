#pragma once

namespace Framework
{
	class Object;

	class IComponent
	{
	public:
		IComponent(Object* owner) : m_owner(owner), m_isActive(true) {};
		virtual ~IComponent() {};
		virtual void Update(float deltaTime) = 0;
		virtual void FixedUpdate(float interval) {};
		virtual void Draw() = 0;

		Object* GetOwner() { return m_owner; }
		void SetActive(bool isActive) { m_isActive = isActive; }
		bool GetActive() { return m_isActive; }

	protected:
		Object* m_owner;
		bool m_isActive;
	};
}