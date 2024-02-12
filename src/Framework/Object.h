#pragma once
#include "IComponent.h"
#include "Editor.h"
#include "Utility/EngineUtility.h"

namespace Framework
{
	/// <summary>
	/// �Q�[�����ɑ��݂���S�I�u�W�F�N�g�̊��N���X
	/// ���̃I�u�W�F�N�g�ɃR���|�[�l���g��ǉ����āA�Q�[�����\������I�u�W�F�N�g���쐬����
	/// </summary>
	class Object
	{
	public:
		Object();
		virtual ~Object()
		{
			m_components.clear();
			m_children.clear();
		}

		/// <summary>
		/// �ΏۃR���|�[�l���g���擾����
		/// </summary>
		/// <typeparam name="T">IComponent�C���^�[�t�F�[�X�����������N���X</typeparam>
		/// <returns>���ɃA�^�b�`����Ă���F�R���|�[�l���g �A�^�b�`����Ă��Ȃ��Fnullptr</returns>
		template<class T>
		T* GetComponent()
		{
			for (auto& comp : m_components)
			{
				if (typeid(*comp.get()) == typeid(T))
					return static_cast<T*>(comp.get());
			}

			return nullptr;
		}

		/// <summary>
		/// �ΏۃR���|�[�l���g��ǉ�����
		/// �����R���|�[�l���g�𕡐��A�^�b�`���邱�Ƃ͕s�\
		/// </summary>
		/// <typeparam name="T">IComponent�C���^�[�t�F�[�X�����������N���X</typeparam>
		/// <param name="owner">���ɃA�^�b�`����Ă���Fnullptr �A�^�b�`����Ă��Ȃ��F�R���|�[�l���g</param>
		/// <returns></returns>
		template<class T>
		T* AddComponent(const std::shared_ptr<Object>& owner)
		{
			for (auto& comp : m_components) {
				if (typeid(*comp.get()) == typeid(T))
					return nullptr;
			}
			m_components.push_back(std::make_shared<T>(owner));

			return static_cast<T*>(m_components.back().get());
		}

		void Start();
		void Update(float deltaTime);
		void FixedUpdate(float inverval);
		void LateUpdate(float deltaTime);
		void Draw();
		void DrawInspector();

		void SetActive(bool isActive);
		bool GetActive();

		/// <summary>
		/// �I�u�W�F�N�g�̍폜�t���O�𗧂Ă�
		/// ���̊֐���ǂ񂾎��_�ł͍폜����Ȃ�
		/// ���̊֐���ǂ񂾃t���[���̏I���ō폜�����
		/// </summary>
		void Destroy();

		/// <summary>
		/// �폜�t���O�������Ă��邩�ǂ�����Ԃ�
		/// </summary>
		/// <returns></returns>
		bool ShouldDestroy();

		/// <summary>
		/// �q�I�u�W�F�N�g�̍폜�t���O�������Ă��邩�ǂ�����Ԃ�
		/// �t�܂ōċA�I�ɌĂяo��
		/// </summary>
		/// <returns></returns>
		bool ShouldDestroyChild();

		void SetName(std::string name);
		const std::string GetName();

		void SetTag(std::string tag);
		const std::string GetTag();

		/// <summary>
		/// �q�I�u�W�F�N�g�Ƃ��Ēǉ�����
		/// ���̊֐����ĂԎ��AObjectManager::CreateObject()�ō쐬�����I�u�W�F�N�g�͓n���Ȃ�����
		/// </summary>
		/// <param name="child"></param>
		void AddChild(Object* child);

		/// <summary>
		/// �q�I�u�W�F�N�g�Ƃ��Ēǉ�����
		/// ���̊֐����ĂԎ��AObjectManager::CreateObject()�ō쐬�����I�u�W�F�N�g�͓n���Ȃ�����
		/// </summary>
		/// <param name="child"></param>
		void AddChild(const std::shared_ptr<Object>& child);

		/// <summary>
		/// �q�I�u�W�F�N�g�̍폜�t���O�𗧂Ă�
		/// </summary>
		/// <param name="child"></param>
		void RemoveChild(Object* child);

		/// <summary>
		/// �q�I�u�W�F�N�g�̍폜�t���O�𗧂Ă�
		/// </summary>
		/// <param name="child"></param>
		void RemoveChild(const std::shared_ptr<Object>& child);

		/// <summary>
		/// index�Ԗڂ̎q�I�u�W�F�N�g���擾����
		/// </summary>
		/// <param name="index">�C���f�b�N�X</param>
		/// <returns></returns>
		std::shared_ptr<Object>& GetChild(size_t index);

		/// <summary>
		/// �S�q�I�u�W�F�N�g���擾����
		/// </summary>
		/// <returns></returns>
		const std::list<std::shared_ptr<Object>>& GetChildren();

		/// <summary>
		/// �S�q�I�u�W�F�N�g���폜����
		/// �폜�t���O�𗧂Ă�
		/// </summary>
		void RemoveAllChildren();

		const std::string GetUUID();

	protected:
		std::list<std::shared_ptr<IComponent>> m_components;
		std::list<std::shared_ptr<Object>> m_children;
		Object* m_parent = nullptr;
		std::string m_name = "Object";
		std::string m_tag = "None";
		std::string m_uuid;
		bool m_isActive = true;		// �I�u�W�F�N�g���A�N�e�B�u���ǂ���
		bool m_isStarted = false;	// Start�֐����Ă΂ꂽ���ǂ���
		bool m_isDestroy = false;	// �I�u�W�F�N�g���폜����邩�ǂ���
		bool m_isDestroyChild = false;	// �q�I�u�W�F�N�g���폜����邩�ǂ���

		/// <summary>
		/// �ċA�I�ɂ��ǂ��Đe�I�u�W�F�N�g�́u�q�I�u�W�F�N�g�폜�t���O�v�𗧂Ă�
		/// </summary>
		void DestroyChild();
	};

	// template�Ŏ������Ă���I�u�W�F�N�g�}�l�[�W���[��
	// GameObject��UIObject�p�Ɏg�������邽�߂̃N���X
	class GameObject : public Object
	{
	public:
		GameObject() = default;
		~GameObject() {}
	};

	class UIObject : public Object
	{
	public:
		UIObject() = default;
		~UIObject() {}
	};
}