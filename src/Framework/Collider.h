#pragma once
#include "IComponent.h"

namespace Framework
{
	/// <summary>
	/// コライダー形状の種類
	/// </summary>
	enum class COLLIDER_TYPE
	{
		RECT,
		CIRCLE,
		NONE,
	};

	/// <summary>
	/// コライダーコンポーネントの基底クラス
	/// </summary>
	class Collider : public IComponent
	{
	public:
		Collider(std::shared_ptr<Object> owner);
		virtual ~Collider();

		virtual void Update(float deltaTime) = 0;
		virtual void Draw() = 0;
		virtual void DrawInspector() {};

		/// <summary>
		/// コライダーの形状を取得する
		/// </summary>
		/// <returns>void*に変換された形状データ</returns>
		virtual void* GetColliderShape() const = 0;

		/// <summary>
		/// コライダーの形状タイプを取得する
		/// この数値を元に、GetColliderShapeで取得したデータをキャストする
		/// </summary>
		/// <returns>形状タイプ</returns>
		COLLIDER_TYPE GetColliderType() const { return m_colliderType; };

		// 必要ならばOnCollisionEnter, OnCollisionStay, OnCollisionExitを実装する

		/// <summary>
		/// 衝突時に呼ばれるコールバック関数を設定する
		/// </summary>
		/// <param name="func"></param>
		void SetOnCollisionCallBack(std::function<void(Collider*)> func);

		/// <summary>
		/// 衝突時に呼ばれるコールバック関数
		/// この関数からSetOnCollisionCallBackで設定した関数が呼ばれる
		/// </summary>
		/// <param name="other">衝突したコライダー</param>
		void OnCollision(Collider* other);

	protected:
		COLLIDER_TYPE m_colliderType = COLLIDER_TYPE::NONE;
		std::function<void(Collider*)> m_onCollisionCallBack = nullptr;
	};
}