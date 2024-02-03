#include "pch.h"
#include "Object.h"
#include "Transform2D.h"
#include "GameObjectManager.h"

#include "imgui.h"

namespace Framework
{
	Object::Object()
	{
		m_components.push_back(std::make_shared<Transform2D>(this));

		// UUIDを生成
		m_uuid = Utility::GenerateUUID();
	}
	void Object::Update(float deltaTime)
	{
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
		// オブジェクトの名前とUUIDを表示
		ImGui::SeparatorText("Object Data");
		ImGui::Text("Name: %s", m_name.c_str());
		ImGui::SameLine();
		ImGui::Checkbox("Active", &m_isActive);

		for (auto& component : m_components)
		{
			component->DrawInspector();
		}
		//for (auto& child : m_children)
		//{
		//	child->DrawInspector();
		//}
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
		m_children.push_back(child);
	}
	void Object::RemoveChild(Object* child)
	{
		m_children.remove(child);
	}
	const std::list<Object*>& Object::GetChildren()
	{
		return m_children;
	}
	const std::string Object::GetUUID()
	{
		return m_uuid;
	}
}