#include "pch.h"
#include "Object.h"
#include "Transform2D.h"

namespace Framework
{
	Object::Object() : m_parent(nullptr), m_children()
	{
		m_components.push_back(std::make_unique<Transform2D>(this));
	}
	void Object::Update(float deltaTime)
	{
		for (auto& component : m_components)
		{
			component->Update(deltaTime);
		}

		for (auto& child : m_children)
		{
			child->Update(deltaTime);
		}
	}
	void Object::Draw()
	{
		// �q�I�u�W�F�N�g�����݂���ꍇ�A
		// �e�I�u�W�F�N�g�̓X�e�[�W�Ȃǂ̔w�i�I�u�W�F�N�g�̉\�������邽�߁A
		// �q�I�u�W�F�N�g���ɕ`�悷��
		for (auto& child : m_children)
		{
			child->Draw();
		}

		for (auto& component : m_components)
		{
			component->Draw();
		}
	}
	Object* Object::Parent()
	{
		return m_parent;
	}
	void Object::SetParent(Object* parent)
	{
		m_parent = parent;
	}
	void Object::AddChild(std::unique_ptr<Object>& child)
	{
		m_children.push_back(std::move(child));
	}
	void Object::SetActive(bool isActive)
	{
		m_isActive = isActive;
	}
	bool Object::GetActive()
	{
		return m_isActive;
	}
}