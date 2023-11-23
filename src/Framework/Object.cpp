#include "pch.h"
#include "Object.h"
#include "Transform2D.h"

namespace Framework
{
	Object::Object()
	{
		m_components.push_back(std::make_unique<Transform2D>(this));
	}
	void Object::Update(float deltaTime)
	{
		for (auto& component : m_components)
		{
			component->Update(deltaTime);
		}
	}
	void Object::Draw()
	{
		// �q�I�u�W�F�N�g�����݂���ꍇ�A
		// �e�I�u�W�F�N�g�̓X�e�[�W�Ȃǂ̔w�i�I�u�W�F�N�g�̉\�������邽�߁A
		// �q�I�u�W�F�N�g���ɕ`�悷��
		for (auto& component : m_components)
		{
			component->Draw();
		}
	}
	void Object::SetActive(bool isActive)
	{
		m_isActive = isActive;
	}
	bool Object::GetActive()
	{
		return m_isActive;
	}
	void Object::SetName(std::string name)
	{
		m_name = name;
	}
	std::string Object::GetName()
	{
		return m_name;
	}
}