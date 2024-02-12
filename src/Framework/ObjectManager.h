#pragma once

namespace Framework
{
	class Object;

	/// <summary>
	/// �I�u�W�F�N�g�̊Ǘ��N���X
	/// </summary>
	/// <typeparam name="T">Object�^���p�����Ă���K�v������</typeparam>
	template<class T>
	class ObjectManager
	{
	public:
		static void Start();
		static void Update(float deltaTime);
		static void FixedUpdate(float interval);
		static void LateUpdate(float deltaTime);

		/// <summary>
		/// �V�K���[�g�I�u�W�F�N�g�𐶐�����
		/// </summary>
		static std::shared_ptr<T> CreateObject();

		/// <summary>
		/// �S���[�g�I�u�W�F�N�g���擾����
		/// </summary>
		/// <returns></returns>
		static std::list<std::shared_ptr<T>>& GetAllObject();

		/// <summary>
		/// ���O�ŃI�u�W�F�N�g����������
		/// �����Ώۂ̓��[�g�̃I�u�W�F�N�g�̂�
		/// �Ώۂ�������Ȃ��ꍇ��nullptr��Ԃ�
		/// </summary>
		/// <param name="name">�����Ώۂ̖��O</param>
		/// <returns>���������F�I�u�W�F�N�g �������Fnullptr</returns>
		static std::shared_ptr<T> FindObject(std::string name);

		/// <summary>
		/// UUID�ŃI�u�W�F�N�g����������
		/// �����Ώۂ̓��[�g�̃I�u�W�F�N�g�̂�
		/// �Ώۂ�������Ȃ��ꍇ��nullptr��Ԃ�
		/// </summary>
		/// <param name="uuid">�����Ώۂ�UUID</param>
		/// <returns>���������F�I�u�W�F�N�g �������Fnullptr</returns>
		static std::shared_ptr<T> FindObjectWithUUID(std::string uuid);

		static void Reset();

	protected:
		ObjectManager() = default;
		virtual ~ObjectManager();

		static std::list<std::shared_ptr<T>> m_objects;
	};


	template<class T>
	inline void ObjectManager<T>::Start()
	{
		for (auto& object : m_objects)
		{
			object->Start();
		}
	}

	template<class T>
	inline void ObjectManager<T>::Update(float deltaTime)
	{
		for (auto& object : m_objects)
		{
			if (object->GetActive())
				object->Update(deltaTime);
		}
	}

	template<class T>
	inline void ObjectManager<T>::FixedUpdate(float interval)
	{
		for (auto& object : m_objects)
		{
			if (object->GetActive())
				object->FixedUpdate(interval);
		}
	}

	template<class T>
	inline void ObjectManager<T>::LateUpdate(float deltaTime)
	{
		// �폜�t���O�������Ă���q�I�u�W�F�N�g���폜
		for (auto& object : m_objects)
		{
			// �폜�Ώۂ̎q�I�u�W�F�N�g���ċA�I�ɒH���č폜
			if (object->ShouldDestroyChild())
			{
				object->LateUpdate(deltaTime);
			}
		}

		// �폜�t���O�������Ă���I�u�W�F�N�g���폜
		m_objects.remove_if([](std::shared_ptr<T> object) { return object->ShouldDestroy(); });
	}

	template<class T>
	inline std::shared_ptr<T> ObjectManager<T>::CreateObject()
	{
		std::shared_ptr<T> object = std::shared_ptr<T>(new T());
		m_objects.push_back(object);

		return object;
	}

	template<class T>
	inline std::list<std::shared_ptr<T>>& ObjectManager<T>::GetAllObject()
	{
		return m_objects;
	}

	template<class T>
	inline std::shared_ptr<T> ObjectManager<T>::FindObject(std::string name)
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
	inline std::shared_ptr<T> ObjectManager<T>::FindObjectWithUUID(std::string uuid)
	{
		for (auto& object : m_objects)
		{
			if (object->GetUUID() == uuid)
			{
				return object;
			}
		}

		return nullptr;
	}

	template<class T>
	inline void ObjectManager<T>::Reset()
	{
		// �S�I�u�W�F�N�g���폜����
		if (!m_objects.empty())
		{
			//for (auto& object : m_objects)
			//{
			//	delete object;
			//}
			m_objects.clear();
		}
	}

	template<class T>
	inline ObjectManager<T>::~ObjectManager()
	{
		if (!m_objects.empty())
		{
			//for (auto& object : m_objects)
			//{
			//	delete object;
			//}
			m_objects.clear();
		}
	}
	// �ÓI�����o�ϐ��̎��̉�
	template<class T>
	std::list<std::shared_ptr<T>> ObjectManager<T>::m_objects;
}


