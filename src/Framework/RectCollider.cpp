#include "pch.h"
#include "RectCollider.h"

#include "Object.h"
#include "Transform2D.h"

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
		// �R���|�[�l���g�̏��L�҂̍��W���擾���čX�V
		m_rect.x = m_owner->GetComponent<Transform2D>()->position.x;
		m_rect.y = m_owner->GetComponent<Transform2D>()->position.y;
	}
	void RectCollider::Draw()
	{
		// �f�o�b�O�p�`��
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

