#pragma once
#include "IComponent.h"
#include "Editor.h"
#include "Utility/EngineUtility.h"

namespace Framework
{
	/// <summary>
	/// ゲーム中に存在する全オブジェクトの基底クラス
	/// このオブジェクトにコンポーネントを追加して、ゲームを構成するオブジェクトを作成する
	/// </summary>
	class Object
	{
	public:
		Object();
		virtual ~Object()
		{
			m_components.clear();
			m_children.clear();
		}

		/// <summary>
		/// 対象コンポーネントを取得する
		/// </summary>
		/// <typeparam name="T">IComponentインターフェースを実装したクラス</typeparam>
		/// <returns>既にアタッチされている：コンポーネント アタッチされていない：nullptr</returns>
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

		/// <summary>
		/// 対象コンポーネントを追加する
		/// 同じコンポーネントを複数アタッチすることは不可能
		/// </summary>
		/// <typeparam name="T">IComponentインターフェースを実装したクラス</typeparam>
		/// <param name="owner">既にアタッチされている：nullptr アタッチされていない：コンポーネント</param>
		/// <returns></returns>
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

		/// <summary>
		/// オブジェクトの削除フラグを立てる
		/// この関数を読んだ時点では削除されない
		/// この関数を読んだフレームの終わりで削除される
		/// </summary>
		void Destroy();

		/// <summary>
		/// 削除フラグが立っているかどうかを返す
		/// </summary>
		/// <returns></returns>
		bool ShouldDestroy();

		/// <summary>
		/// 子オブジェクトの削除フラグが立っているかどうかを返す
		/// 葉まで再帰的に呼び出す
		/// </summary>
		/// <returns></returns>
		bool ShouldDestroyChild();

		void SetName(std::string name);
		const std::string GetName();

		void SetTag(std::string tag);
		const std::string GetTag();

		/// <summary>
		/// 子オブジェクトとして追加する
		/// この関数を呼ぶ時、ObjectManager::CreateObject()で作成したオブジェクトは渡さないこと
		/// </summary>
		/// <param name="child"></param>
		void AddChild(Object* child);

		/// <summary>
		/// 子オブジェクトとして追加する
		/// この関数を呼ぶ時、ObjectManager::CreateObject()で作成したオブジェクトは渡さないこと
		/// </summary>
		/// <param name="child"></param>
		void AddChild(const std::shared_ptr<Object>& child);

		/// <summary>
		/// 子オブジェクトの削除フラグを立てる
		/// </summary>
		/// <param name="child"></param>
		void RemoveChild(Object* child);

		/// <summary>
		/// 子オブジェクトの削除フラグを立てる
		/// </summary>
		/// <param name="child"></param>
		void RemoveChild(const std::shared_ptr<Object>& child);

		/// <summary>
		/// index番目の子オブジェクトを取得する
		/// </summary>
		/// <param name="index">インデックス</param>
		/// <returns></returns>
		std::shared_ptr<Object>& GetChild(size_t index);

		/// <summary>
		/// 全子オブジェクトを取得する
		/// </summary>
		/// <returns></returns>
		const std::list<std::shared_ptr<Object>>& GetChildren();

		/// <summary>
		/// 全子オブジェクトを削除する
		/// 削除フラグを立てる
		/// </summary>
		void RemoveAllChildren();

		const std::string GetUUID();

	protected:
		std::list<std::shared_ptr<IComponent>> m_components;
		std::list<std::shared_ptr<Object>> m_children;
		Object* m_parent = nullptr;
		std::string m_name = "Object";
		std::string m_tag = "None";
		std::string m_uuid;
		bool m_isActive = true;		// オブジェクトがアクティブかどうか
		bool m_isStarted = false;	// Start関数が呼ばれたかどうか
		bool m_isDestroy = false;	// オブジェクトが削除されるかどうか
		bool m_isDestroyChild = false;	// 子オブジェクトが削除されるかどうか

		/// <summary>
		/// 再帰的にたどって親オブジェクトの「子オブジェクト削除フラグ」を立てる
		/// </summary>
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