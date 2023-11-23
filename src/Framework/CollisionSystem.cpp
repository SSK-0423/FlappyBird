#include "pch.h"
#include "CollisionSystem.h"
#include "IComponent.h"
#include "Collider.h"
#include "RectCollider.h"

namespace Framework
{
	// �ÓI�����o�ϐ��̎��̉�
	std::list<std::shared_ptr<Collider>> CollisionSystem::m_colliders;

	void CollisionSystem::Update(float deltaTime)
	{
		// �������蔻��
		// TODO: ��蕡�G�ȃQ�[������������ۂ͋�ԕ�������������
		for (auto& collider1 : m_colliders)
		{
			for (auto& collider2 : m_colliders)
			{
				// �����R���C�_�[���m�͔��肵�Ȃ�(�A�h���X�Ŕ���)
				if (&collider1 == &collider2)
				{
					continue;
				}

				// �����蔻�茟�o
				if (CollisionDetection(*collider1, *collider2))
				{
					// �Փ˂��Ă����ꍇ�̓R�[���o�b�N���Ă�

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
		Rect* rect1 = reinterpret_cast<Rect*>(collider1.GetColliderShape());
		Rect* rect2 = reinterpret_cast<Rect*>(collider2.GetColliderShape());

		// TODO: ��`���m�̔���

		return false;
	}
}
