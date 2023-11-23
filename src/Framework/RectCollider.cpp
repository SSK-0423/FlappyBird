#include "pch.h"
#include "RectCollider.h"

namespace Framework
{
	RectCollider::RectCollider(Object* owner)
		: Collider(owner), m_rect(0, 0, 1, 1)
	{
		m_colliderType = COLLIDER_TYPE::RECT;
	}
	RectCollider::~RectCollider()
	{
	}
	void RectCollider::Update(float deltaTime)
	{
		// コンポーネントの所有者の座標を取得して更新
	}
	void RectCollider::Draw()
	{
		// デバッグ用描画
	}
	void* RectCollider::GetColliderShape() const
	{
		// メモ: ここで返すのはconstじゃないといけないので、const_castで無理やりconstを外している
		// キャストコストがあるので、できればconstを外さない方法を考えたい
		return reinterpret_cast<void*>(const_cast<Rect*>(&m_rect));
	}
}

