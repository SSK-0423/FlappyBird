#pragma once
namespace Framework
{
	class Object;

	template<class T>
	class ObjectManager
	{
	public:
		static void Update(float deltaTime);

		static T* CreateObject();

		//static void AddObject(std::shared_ptr<T>& object);
		//static void AddObject(T* object);

		//static std::list<std::shared_ptr<T>>& GetAllObject();
		static std::list<T*>& GetAllObject();

		//static std::shared_ptr<T> FindObject(std::string name);
		static T* FindObject(std::string name);

		static void Reset();

	protected:
		ObjectManager() = default;
		virtual ~ObjectManager();

		static std::list<T*> m_objects;
	};

	template<class T>
	inline void ObjectManager<T>::Update(float deltaTime)
	{
		for (auto& object : m_objects)
		{
			object->Update(deltaTime);
		}
	}

	template<class T>
	inline T* ObjectManager<T>::CreateObject()
	{
		T* object = new T();
		m_objects.push_back(object);

		return object;
	}

	//template<class T>
	//inline void ObjectManager<T>::AddObject(std::shared_ptr<T>& object)
	//{
	//	//m_objects.push_back(object);
	//}

	//template<class T>
	//inline void ObjectManager<T>::AddObject(T* object)
	//{
	//	m_objects.push_back(object);
	//}

	//template<class T>
	//inline std::list<std::shared_ptr<T>>& ObjectManager<T>::GetAllObject()
	//{
	//	return m_objects;
	//}

	template<class T>
	inline std::list<T*>& ObjectManager<T>::GetAllObject()
	{
		return m_objects;
	}


	//template<class T>
	//inline std::shared_ptr<T> ObjectManager<T>::FindObject(std::string name)
	//{
	//	for (auto& object : m_objects)
	//	{
	//		if (object->GetName() == name)
	//		{
	//			return object;
	//		}
	//	}

	//	return nullptr;
	//}


	template<class T>
	inline T* ObjectManager<T>::FindObject(std::string name)
	{
		for (auto& object : m_objects)
		{
			if (object->GetName() == name)
			{
				return object;
			}
		}

		return nullptr;
	}

	template<class T>
	inline void ObjectManager<T>::Reset()
	{
		// 全オブジェクトを削除する
		if (!m_objects.empty())
		{
			m_objects.clear();
		}
	}

	template<class T>
	inline ObjectManager<T>::~ObjectManager()
	{
		if (!m_objects.empty())
		{
			m_objects.clear();
		}
	}
	// 静的メンバ変数の実体化
	template<class T>
	std::list<T*> ObjectManager<T>::m_objects;
	//std::list<std::shared_ptr<T>> ObjectManager<T>::m_objects;
}


