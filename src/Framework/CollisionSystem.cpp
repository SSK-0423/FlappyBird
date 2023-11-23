#include "pch.h"
#include "CollisionSystem.h"
#include "IComponent.h"
#include "Collider.h"
#include "RectCollider.h"

namespace Framework
{
	// 静的メンバ変数の実体化
	std::list<std::shared_ptr<Collider>> CollisionSystem::m_colliders;

	void CollisionSystem::Update(float deltaTime)
	{
		// 総当たり判定
		// TODO: より複雑なゲームを実装する際は空間分割を実装する
		for (auto& collider1 : m_colliders)
		{
			for (auto& collider2 : m_colliders)
			{
				// 同じコライダー同士は判定しない(アドレスで判定)
				if (&collider1 == &collider2)
				{
					continue;
				}

				// 当たり判定検出
				if (CollisionDetection(*collider1, *collider2))
				{
					// 衝突していた場合はコールバックを呼ぶ

				}
			}
		}
	}

	void CollisionSystem::AddCollider(std::shared_ptr<Collider>& collider)
	{
		m_colliders.push_back(collider);
	}

	void CollisionSystem::Reset()
	{
		m_colliders.clear();
	}

	CollisionSystem::~CollisionSystem()
	{
		m_colliders.clear();
	}
	bool CollisionSystem::CollisionDetection(const Collider& collider1, const Collider& collider2)
	{
		// 矩形同士の判定
		if (collider1.GetColliderType() == COLLIDER_TYPE::RECT &&
			collider2.GetColliderType() == COLLIDER_TYPE::RECT)
		{
			return RectRect(collider1, collider2);
		}

		return false;
	}
	bool CollisionSystem::RectRect(const Collider& collider1, const Collider& collider2)
	{

		// 必要なデータを取得する
		Rect* rect1 = reinterpret_cast<Rect*>(collider1.GetColliderShape());
		Rect* rect2 = reinterpret_cast<Rect*>(collider2.GetColliderShape());

		// TODO: 矩形同士の判定

		return false;
	}
}
