#include "Scene.h"
#include "IRenderer.h"
#include "Transform2D.h"
#include "GameObjectManager.h"
#include "UIObjectManager.h"
#include "CollisionSystem.h"

namespace Framework
{
	// �ÓI�����o�ϐ��̎��̉�
	std::shared_ptr<GameObject> Scene::m_cameraObject = nullptr;

	Scene::Scene()
	{
		// �V�[�����C���X�^���X������邽�тɃJ���������������̂�h��
		if (m_cameraObject == nullptr)
		{
			m_cameraObject = std::make_shared<GameObject>();
			m_cameraObject->AddComponent<Camera>(m_cameraObject.get());
		}
	}
	void Scene::Update(float deltaTime)
	{
		// �I�u�W�F�N�g�S�̂̍X�V
		GameObjectManager::Update(deltaTime);

		// �R���W��������
		CollisionSystem::Update(deltaTime);

		// UI�̍X�V
		UIObjectManager::Update(deltaTime);

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
	}
	const Camera& Scene::GetCamera()
	{
		return *m_cameraObject->GetComponent<Camera>();
	}
}