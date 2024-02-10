#pragma once
#include "IComponent.h"
#include "Editor.h"
#include "Utility/EngineUtility.h"

namespace Framework
{
	class Object
	{
	public:
		Object();
		virtual ~Object()
		{
			m_components.clear();
			m_children.clear();
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
		T* AddComponent(const std::shared_ptr<Object>& owner)
		{
			for (auto& comp : m_components) {
				if (typeid(*comp.get()) == typeid(T))
					return nullptr;
			}
			m_components.push_back(std::make_shared<T>(owner));

			return static_cast<T*>(m_components.back().get());
		}

		void Start();
		void Update(float deltaTime);
		void FixedUpdate(float inverval);
		void LateUpdate(float deltaTime);
		void Draw();
		void DrawInspector();

		void SetActive(bool isActive);
		bool GetActive();

		void Destroy();
		bool ShouldDestroy();
		bool ShouldDestroyChild();

		void SetName(std::string name);
		const std::string GetName();

		void SetTag(std::string tag);
		const std::string GetTag();

		void AddChild(Object* child);
		void AddChild(const std::shared_ptr<Object>& child);
		void RemoveChild(Object* child);
		void RemoveChild(const std::shared_ptr<Object>& child);
		void RemoveAllChildren();
		const std::list<std::shared_ptr<Object>>& GetChildren();

		const std::string GetUUID();

	protected:
		std::list<std::shared_ptr<IComponent>> m_components;
		std::list<std::shared_ptr<Object>> m_children;
		Object* m_parent = nullptr;
		std::string m_name = "Object";
		std::string m_tag = "None";
		std::string m_uuid;
		bool m_isActive = true;
		bool m_isStarted = false;
		bool m_isDestroy = false;
		bool m_isDestroyChild = false;

		void DestroyChild();
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