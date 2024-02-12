#pragma once

namespace Framework
{
	class Collider;

	/// <summary>
	/// 衝突判定を行うクラス
	/// </summary>
	class CollisionSystem
	{
	public:
		static void Update(float deltaTime);

		/// <summary>
		/// コライダーを衝突判定対象に追加する
		/// </summary>
		/// <param name="collider">コライダー</param>
		static void AddCollider(Collider* collider);

		/// <summary>
		/// コライダーを衝突判定対象から削除する
		/// </summary>
		/// <param name="collider">コライダー</param>
		static void RemoveCollider(Collider* collider);

		/// <summary>
		/// 衝突判定対象を全て削除する
		/// ColliderコンポーネントはObjectの破棄時に削除されるため、
		/// この関数内ではdeleteは行わない
		/// </summary>
		static void Reset();

	private:
		CollisionSystem() = default;
		~CollisionSystem();

		/// <summary>
		/// 衝突判定を行う
		/// </summary>
		/// <param name="collider1">コライダー1</param>
		/// <param name="collider2">コライダー2</param>
		/// <returns>true：衝突している false：衝突していない</returns>
		static bool CollisionDetection(const Collider& collider1, const Collider& collider2);

		/// <summary>
		/// 矩形同士の衝突判定を行う
		/// </summary>
		/// <param name="collider1">矩形1</param>
		/// <param name="collider2">矩形2</param>
		/// <returns>true：衝突している false：衝突していない</returns>
		static bool RectRect(const Collider& collider1, const Collider& collider2);

		// 必要があれば他の判定も追加

		static std::list<Collider*> m_colliders;	// 衝突判定対象となるコライダーのリスト
	};
}