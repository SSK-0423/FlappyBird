#pragma once

namespace Framework
{
	class Object;

	/// <summary>
	/// コンポーネントのインターフェース
	/// </summary>
	class IComponent
	{
	public:
		IComponent(const std::shared_ptr<Object>& owner) : m_owner(owner), m_isActive(true) {};
		virtual ~IComponent() {};
		virtual void Start() {};
		virtual void Update(float deltaTime) = 0;
		virtual void FixedUpdate(float interval) {};
		virtual void Draw() = 0;
		virtual void DrawInspector() {};

		std::shared_ptr<Object> GetOwner() { return m_owner.lock(); }
		void SetActive(bool isActive) { m_isActive = isActive; }
		bool GetActive() { return m_isActive; }

	protected:
		std::weak_ptr<Object> m_owner;
		bool m_isActive;
	};
}