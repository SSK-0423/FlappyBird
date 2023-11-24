#include "pch.h"
#include "CollisionSystem.h"
#include "IComponent.h"
#include "Collider.h"
#include "RectCollider.h"

#include <DirectXMath.h>

using namespace DirectX;

namespace Framework
{
	// 静的メンバ変数の実体化
	std::list<Collider*> CollisionSystem::m_colliders;

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
					OutputDebugStringA("Collision!!!!!!!!!!\n");
				}
			}
		}
	}

	void CollisionSystem::AddCollider(Collider* collider)
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
		const Rect* rect1 = reinterpret_cast<Rect*>(collider1.GetColliderShape());
		const Rect* rect2 = reinterpret_cast<Rect*>(collider2.GetColliderShape());

		// TODO: 矩形同士の判定
		XMFLOAT2 rect1Center = rect1->GetCenter();
		XMFLOAT2 rect2Center = rect2->GetCenter();

		// 各軸毎の距離を計算
		float distanceX = std::fabs(rect1Center.x - rect2Center.x);
		float distanceY = std::fabs(rect1Center.y - rect2Center.y);

		// 各軸毎のサイズの合計値の半分を計算
		float halfWidthSum = (rect1->width + rect2->width) / 2.f;
		float halfHeightSum = (rect1->height + rect2->height) / 2.f;

		// 各軸毎の距離が合計値の半分より小さい場合は当たっている
		if (distanceX < halfWidthSum &&
			distanceY < halfHeightSum)
		{
			return true;
		}

		return false;
	}
}
