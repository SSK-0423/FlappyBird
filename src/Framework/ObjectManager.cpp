//#include "pch.h"
//#include "ObjectManager.h"
//#include "Object.h"
//
//namespace Framework
//{
//	// �ÓI�����o�ϐ��̎��̉�
//	std::list<std::shared_ptr<Object>> ObjectManager::m_objects;
//
//	void ObjectManager::Update(float deltaTime)
//	{
//	}
//	void ObjectManager::AddObject(Object* object)
//	{
//		m_objects.push_back(std::shared_ptr<Object>(object));
//	}
//	std::list<std::shared_ptr<Object>>& ObjectManager::GetAllObject()
//	{
//		return m_objects;
//	}
//	std::shared_ptr<Object> ObjectManager::FindObject(std::string name)
//	{
//		// ���O����v����I�u�W�F�N�g��T��
//		for (auto& object : m_objects)
//		{
//			if (object->GetName() == name)
//			{
//				return object;
//			}
//		}
//	}
//	void ObjectManager::Reset()
//	{
//		m_objects.clear();
//	}
//	ObjectManager::~ObjectManager()
//	{
//		Reset();
//	}
//}
//
