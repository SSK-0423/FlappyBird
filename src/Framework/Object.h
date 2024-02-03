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
		void FixedUpdate(float inverval);
		void Draw();
		void DrawInspector();

		void SetActive(bool isActive);
		bool GetActive();

		void SetName(std::string name);
		const std::string GetName();

		void SetTag(std::string tag);
		const std::string GetTag();

		void AddChild(Object* child);
		const std::list<Object*>& GetChildren();

		const std::string GetUUID();

	protected:
		std::list<std::shared_ptr<IComponent>> m_components;
		std::list<Object*> m_children;
		Object* m_parent = nullptr;
		std::string m_name = "Object";
		std::string m_tag = "None";
		std::string m_uuid;
		bool m_isActive = true;
	};

	// templateで実装してあるオブジェクトマネージャーを
	// GameObjectとUIObject用に使い分けるためのクラス
	class GameObject : public Object
	{
	public:
		GameObject() = default;
		~GameObject() {}
	};

	class UIObject : public Object
	{
	public:
		UIObject() = default;
		~UIObject() {}
	};
}