#pragma once

namespace Framework
{
	class Object;

	class IComponent
	{
	public:
		IComponent(Object* owner) : m_owner(owner) {};
		virtual ~IComponent() {};
		virtual void Update(float deltaTime) = 0;
		virtual void Draw() = 0;

		// �R�[���o�b�N���󂯎��֐�
		virtual void RecieveCallBackMessage(const std::string& message) {};

		Object* GetOwner() { return m_owner; }

	protected:
		Object* m_owner;
	};
}