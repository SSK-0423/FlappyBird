#pragma once

namespace Framework
{
	class Object;

	class IComponent
	{
	public:
		IComponent(Object* owner) : m_owner(owner) {};
		virtual ~IComponent() { OutputDebugStringA("IComponent Destructor\n"); };

	protected:
		Object* m_owner;

	public:
		virtual void Update(float deltaTime) = 0;
		virtual void Draw() = 0;
	};
}