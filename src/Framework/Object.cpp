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
		// 子オブジェクトが存在する場合、
		// 親オブジェクトはステージなどの背景オブジェクトの可能性があるため、
		// 子オブジェクトを先に描画する
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