#include "pch.h"
#include "CollisionSystem.h"
#include "IComponent.h"
#include "Collider.h"
#include "RectCollider.h"
#include "Object.h"

#include "Editor.h"

#include <DirectXMath.h>

using namespace DirectX;

namespace Framework
{
	// �ÓI�����o�ϐ��̎��̉�
	std::list<Collider*> CollisionSystem::m_colliders;

	void CollisionSystem::Update(float deltaTime)
	{
		// �������蔻��
		// TODO: ��蕡�G�ȃQ�[������������ۂ͋�ԕ�������������
		// �����O(N^2)�̌v�Z�ʂȂ̂ŁA�I�u�W�F�N�g����������Əd���Ȃ�
		for (auto& collider1 : m_colliders)
		{
			for (auto& collider2 : m_colliders)
			{
				// TODO: �Փ˂���R���C�_�[�̕\�����

				// ��A�N�e�B�u�ȃR���C�_�[�͔��肵�Ȃ�
				if (!collider1->GetOwner()->GetActive() ||
					!collider2->GetOwner()->GetActive())
				{
					continue;
				}

				// �����R���C�_�[���m�͔��肵�Ȃ�(�A�h���X�Ŕ���)
				if (&collider1 == &collider2)
				{
					continue;
				}

				// �����^�O���m�͔��肵�Ȃ�
				if (collider1->GetOwner()->GetTag() == collider2->GetOwner()->GetTag())
				{
					continue;
				}

				// �����蔻�茟�o
				if (CollisionDetection(*collider1, *collider2))
				{
					collider1->OnCollision(collider2);
					collider2->OnCollision(collider1);
				}
			}
		}
	}

	void CollisionSystem::AddCollider(Collider* collider)
	{
		m_colliders.push_back(collider);
	}

	void CollisionSystem::RemoveCollider(Collider* collider)
	{
		m_colliders.remove(collider);
	}

	void CollisionSystem::Reset()
	{
		// std::list��clear�ł�delete�͌Ă΂�Ȃ����A
		// GameObjectManager, UIObjectManager��Reset�ŃR���C�_�[�͍폜�����̂ŁA
		// �����ł�delete�͌Ă΂Ȃ�
		m_colliders.clear();
	}

	CollisionSystem::~CollisionSystem()
	{
		m_colliders.clear();
	}

	bool CollisionSystem::CollisionDetection(const Collider& collider1, const Collider& collider2)
	{
		// ��`���m�̔���
		if (collider1.GetColliderType() == COLLIDER_TYPE::RECT &&
			collider2.GetColliderType() == COLLIDER_TYPE::RECT)
		{
			return RectRect(collider1, collider2);
		}

		return false;
	}
	bool CollisionSystem::RectRect(const Collider& collider1, const Collider& collider2)
	{
		// �K�v�ȃf�[�^���擾����
		const Rect* rect1 = reinterpret_cast<Rect*>(collider1.GetColliderShape());
		const Rect* rect2 = reinterpret_cast<Rect*>(collider2.GetColliderShape());

		// ��`�̒��S���W���擾
		XMFLOAT2 rect1Center = rect1->GetCenter();
		XMFLOAT2 rect2Center = rect2->GetCenter();

		// �e�����̋������v�Z
		float distanceX = std::fabs(rect1Center.x - rect2Center.x);
		float distanceY = std::fabs(rect1Center.y - rect2Center.y);

		// �e�����̃T�C�Y�̍��v�l�̔������v�Z
		float halfWidthSum = (rect1->width + rect2->width) / 2.f;
		float halfHeightSum = (rect1->height + rect2->height) / 2.f;

		// �e�����̋��������v�l�̔�����菬�����ꍇ�͓������Ă���
		if (distanceX < halfWidthSum &&
			distanceY < halfHeightSum)
		{
			return true;
		}

		return false;
	}
}

