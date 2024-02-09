#include "pch.h"
#include "RectCollider.h"

#include "Object.h"
#include "Transform2D.h"
#include "SpriteRenderer.h"
#include "CollisionSystem.h"

namespace Framework
{
	RectCollider::RectCollider(std::shared_ptr<Object> owner)
		: Collider(owner), m_rect(0, 0, 1, 1, false)
	{
		m_colliderType = COLLIDER_TYPE::RECT;
	}
	RectCollider::~RectCollider()
	{
	}
	void RectCollider::Update(float deltaTime)
	{
		// コンポーネントの所有者の座標を取得して更新
		m_rect.x = m_owner.lock()->GetComponent<Transform2D>()->position.x;
		m_rect.y = m_owner.lock()->GetComponent<Transform2D>()->position.y;
	}
	void RectCollider::Draw()
	{
		// デバッグ用描画
	}
	void RectCollider::DrawInspector()
	{
		if (ImGui::CollapsingHeader("RectCollider"))
		{
			// サイズ
			ImGui::Text("Size");
			ImGui::SameLine();
			ImGui::InputFloat("Width", &m_rect.width);
			ImGui::SameLine();
			ImGui::InputFloat("Height", &m_rect.height);

			// 座標
			ImGui::Text("Position");
			ImGui::SameLine();
			ImGui::InputFloat("X", &m_rect.x);
			ImGui::SameLine();
			ImGui::InputFloat("Y", &m_rect.y);
		}
	}
	void* RectCollider::GetColliderShape() const
	{
		// メモ: ここで返すのはconstじゃないといけないので、const_castで無理やりconstを外している
		// キャストコストがあるので、できればconstを外さない方法を考えたい
		return reinterpret_cast<void*>(const_cast<Rect*>(&m_rect));
	}
	void RectCollider::SetRectSize(float width, float height)
	{
		m_rect.width = width;
		m_rect.height = height;
	}
}

