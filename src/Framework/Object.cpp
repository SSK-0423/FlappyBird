#include "pch.h"
#include "Object.h"
#include "Transform2D.h"

#include "GameObjectManager.h"
#include "UIObjectManager.h"

#include "imgui.h"

namespace Framework
{
	Object::Object()
	{
		// �ꎞ�I�u�W�F�N�g����������Ă��܂�
		// ���ꂪ�f���[�g����邱�ƂŃf�X�g���N�^���Ă΂�A�������A�N�Z�X�ᔽ����������
		//this->AddComponent<Transform2D>(std::shared_ptr<Object>(this));

		// UUID�𐶐�
		m_uuid = Utility::GenerateUUID();

		// �������擾����
		std::shared_ptr<Object> self = GameObjectManager::FindObjectWithUUID(m_uuid);

		if (self == nullptr)
		{
			self = UIObjectManager::FindObjectWithUUID(m_uuid);
		}

		this->AddComponent<Transform2D>(self);
	}
	void Object::Start()
	{
		// ����Start����Ă����牽�����Ȃ�
		if (m_isStarted)
		{
			return;
		}

		for (auto& component : m_components)
		{
			component->Start();
		}

		for (auto& child : m_children)
		{
			child->Start();
		}

		m_isStarted = true;
	}
	void Object::Update(float deltaTime)
	{
		// ���I�ɐ������ꂽ�I�u�W�F�N�g��Start���Ă΂�Ă��Ȃ��̂ł�����Start���Ă�
		if (!m_isStarted)
		{
			Start();
		}

		for (auto& component : m_components)
		{
			if (component->GetActive())
				component->Update(deltaTime);
		}

		for (auto& child : m_children)
		{
			if (child->GetActive())
				child->Update(deltaTime);
		}
	}
	void Object::FixedUpdate(float interval)
	{
		for (auto& component : m_components)
		{
			if (component->GetActive())
				component->FixedUpdate(interval);
		}

		for (auto& child : m_children)
		{
			if (child->GetActive())
				child->FixedUpdate(interval);
		}
	}
	void Object::Draw()
	{
		for (auto& component : m_components)
		{
			if (component->GetActive())
				component->Draw();
		}

		for (auto& child : m_children)
		{
			if (child->GetActive())
				child->Draw();
		}
	}
	void Object::DrawInspector()
	{
		// �I�u�W�F�N�g�̖��O��UUID��\��
		ImGui::SeparatorText("Object Data");
		ImGui::Checkbox("Active", &m_isActive);
		ImGui::Text("Name: %s", m_name.c_str());
		ImGui::Text("UUID: %s", m_uuid.c_str());
		ImGui::Text("Tag: %s", m_tag.c_str());

		for (auto& component : m_components)
		{
			component->DrawInspector();
		}
	}
	void Object::SetActive(bool isActive)
	{
		m_isActive = isActive;

		for (auto& child : m_children)
		{
			child->SetActive(m_isActive);
		}
	}
	bool Object::GetActive()
	{
		return m_isActive;
	}
	void Object::SetName(std::string name)
	{
		m_name = name;
	}
	const std::string Object::GetName()
	{
		return m_name;
	}
	void Object::SetTag(std::string tag)
	{
		m_tag = tag;
	}
	const std::string Object::GetTag()
	{
		return m_tag;
	}
	void Object::AddChild(Object* child)
	{
		m_children.push_back(std::shared_ptr<Object>(child));
	}
	void Object::AddChild(const std::shared_ptr<Object>& child)
	{
		m_children.push_back(child);
	}
	void Object::RemoveChild(Object* child)
	{
		m_children.remove(std::shared_ptr<Object>(child));
	}
	void Object::RemoveChild(const std::shared_ptr<Object>& child)
	{
		m_children.remove(child);
	}
	void Object::RemoveAllChildren()
	{
		m_children.clear();
	}
	const std::list<std::shared_ptr<Object>>& Object::GetChildren()
	{
		return m_children;
	}
	const std::string Object::GetUUID()
	{
		return m_uuid;
	}
}