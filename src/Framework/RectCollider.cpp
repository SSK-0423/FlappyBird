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
		// �R���|�[�l���g�̏��L�҂̍��W���擾���čX�V
		m_rect.x = m_owner.lock()->GetComponent<Transform2D>()->position.x;
		m_rect.y = m_owner.lock()->GetComponent<Transform2D>()->position.y;
	}
	void RectCollider::Draw()
	{
		// �f�o�b�O�p�`��
	}
	void RectCollider::DrawInspector()
	{
		if (ImGui::CollapsingHeader("RectCollider"))
		{
			// �T�C�Y
			ImGui::Text("Size");
			ImGui::SameLine();
			ImGui::InputFloat("Width", &m_rect.width);
			ImGui::SameLine();
			ImGui::InputFloat("Height", &m_rect.height);

			// ���W
			ImGui::Text("Position");
			ImGui::SameLine();
			ImGui::InputFloat("X", &m_rect.x);
			ImGui::SameLine();
			ImGui::InputFloat("Y", &m_rect.y);
		}
	}
	void* RectCollider::GetColliderShape() const
	{
		// ����: �����ŕԂ��̂�const����Ȃ��Ƃ����Ȃ��̂ŁAconst_cast�Ŗ������const���O���Ă���
		// �L���X�g�R�X�g������̂ŁA�ł����const���O���Ȃ����@���l������
		return reinterpret_cast<void*>(const_cast<Rect*>(&m_rect));
	}
	void RectCollider::SetRectSize(float width, float height)
	{
		m_rect.width = width;
		m_rect.height = height;
	}
}

