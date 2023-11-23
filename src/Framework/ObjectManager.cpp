#include "pch.h"
#include "ObjectManager.h"
#include "Object.h"

namespace Framework
{
	// 静的メンバ変数の実体化
	std::list<std::shared_ptr<Object>> ObjectManager::m_objects;

	ObjectManager::~ObjectManager()
	{
		m_objects.clear();
	}

	void ObjectManager::Update(float deltaTime)
	{
		for (auto& object : m_objects)
		{
			object->Update(deltaTime);
		}
	}

	void ObjectManager::AddObject(std::shared_ptr<Object>& object)
	{
		m_objects.push_back(object);
	}

	std::list<std::shared_ptr<Object>>& ObjectManager::GetAllObject()
	{
		return m_objects;
	}

	std::shared_ptr<Object> ObjectManager::FindObject(std::string name)
	{
		// 名前が一致するオブジェクトを探す
		for (auto& object : m_objects)
		{
			if (object->GetName() == name)
			{
				return object;
			}
		}
	}
	void ObjectManager::Reset()
	{
		m_objects.clear();
	}
}
