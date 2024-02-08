#include "Scene.h"
#include "IRenderer.h"
#include "Transform2D.h"
#include "GameObjectManager.h"
#include "UIObjectManager.h"
#include "SoundManager.h"
#include "CollisionSystem.h"

namespace Framework
{
	// �ÓI�����o�ϐ��̎��̉�
	std::shared_ptr<GameObject> Scene::m_cameraObject = nullptr;

	Scene::Scene() : m_fixedUpdateInterval(0.02f), m_elapsedTime(0.f)
	{
		// �V�[�����C���X�^���X������邽�тɃJ���������������̂�h��
		if (m_cameraObject == nullptr)
		{
			m_cameraObject = std::shared_ptr<GameObject>(new GameObject());
			m_cameraObject->AddComponent<Camera>(m_cameraObject);
		}
	}
	void Scene::Start()
	{
		// �I�u�W�F�N�g�S�̂̊J�n
		GameObjectManager::Start();

		// UI�̊J�n
		UIObjectManager::Start();
	}
	void Scene::Update(float deltaTime)
	{
		// �I�u�W�F�N�g�S�̂̍X�V
		GameObjectManager::Update(deltaTime);

		// �R���W��������
		CollisionSystem::Update(deltaTime);

		// UI�̍X�V
		UIObjectManager::Update(deltaTime);

		// ���Ԋu�N����FixedUpdate���Ăяo��
		// �ʃX���b�h�ŏ�������ׂ��H
		m_elapsedTime += deltaTime;
		if (m_elapsedTime >= m_fixedUpdateInterval)
		{
			GameObjectManager::FixedUpdate(m_fixedUpdateInterval);
			m_elapsedTime = 0.0f;
		}
	}
	void Scene::LateUpdate(float deltaTime)
	{
	}
	void Scene::Final()
	{
		// �S�ẴQ�[���I�u�W�F�N�g���폜
		GameObjectManager::Reset();

		// �S�Ă�UI�I�u�W�F�N�g���폜
		UIObjectManager::Reset();

		// �R���W�����V�X�e�������Z�b�g
		CollisionSystem::Reset();

		// �T�E���h�����Z�b�g
		SoundManager::Reset();
	}
	const Camera& Scene::GetCamera()
	{
		return *m_cameraObject->GetComponent<Camera>();
	}
}