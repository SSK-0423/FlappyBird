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
			m_components.shrink_to_fit();
			m_children.clear();
			m_children.shrink_to_fit();
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
			auto component = std::make_unique<T>(owner);
			m_components.push_back(std::move(component));

			return static_cast<T*>(m_components[m_components.size() - 1].get());
		}

		void Update(float deltaTime);
		void Draw();

		Object* Parent();
		void SetParent(Object* parent);
		void AddChild(std::unique_ptr<Object>& child);
		void SetActive(bool isActive);
		bool GetActive();

	protected:
		std::vector<std::unique_ptr<IComponent>> m_components;
		Object* m_parent = nullptr;
		std::vector<std::unique_ptr<Object>> m_children;
		bool m_isActive = true;
	};

	// Object‚É•Ê–¼‚ð‚Â‚¯‚é
	using GameObject = Object;
	using GUIObject = Object;
}