#pragma once
#include "IComponent.h"

namespace Framework
{
	class Object
	{
	public:
		Object();
		virtual ~Object()
		{
			m_components.clear();
		}

		template<class T>
		T* GetComponent()
		{
			for (auto& comp : m_components)
			{
				if (typeid(*comp.get()) == typeid(T))
					return static_cast<T*>(comp.get());
			}

			return nullptr;
		}

		template<class T>
		T* AddComponent(Object* owner)
		{
			for (auto& comp : m_components) {
				if (typeid(*comp.get()) == typeid(T))
					return nullptr;
			}
			m_components.push_back(std::make_shared<T>(owner));

			return static_cast<T*>(m_components.back().get());
		}

		void Update(float deltaTime);
		void Draw();

		void SetActive(bool isActive);
		bool GetActive();

		void SetName(std::string name);
		std::string GetName();

	protected:
		std::list<std::shared_ptr<IComponent>> m_components;
		std::string m_name = "Object";
		bool m_isActive = true;
	};

	// Object‚É•Ê–¼‚ð‚Â‚¯‚é
	using GameObject = Object;
	using GUIObject = Object;
}