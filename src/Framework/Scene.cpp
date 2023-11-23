#include "Scene.h"
#include "IRenderer.h"
#include "Transform2D.h"
#include "ObjectManager.h"
#include "CollisionSystem.h"

namespace Framework
{
	std::unique_ptr<GameObject> Scene::m_cameraObject = nullptr;

	Scene::Scene()
	{
		// �V�[�����C���X�^���X������邽�тɃJ���������������̂�h��
		if (m_cameraObject == nullptr)
		{
			m_cameraObject = std::make_unique<GameObject>();
			m_cameraObject->AddComponent<Camera>(m_cameraObject.get());
		}
	}
	void Scene::Update(float deltaTime)
	{
		// �I�u�W�F�N�g�S�̂̍X�V
		ObjectManager::Update(deltaTime);

		// �R���W��������
		CollisionSystem::Update(deltaTime);

		// UI�̍X�V
		for (auto& canvas : m_canvases)
		{
			canvas->Update(deltaTime);
		}
	}
	void Scene::LateUpdate(float deltaTime)
	{
	}
	const std::vector<std::unique_ptr<Canvas>>& Scene::GetCanvases() const
	{
		return m_canvases;
	}
	const Camera& Scene::GetCamera()
	{
		return *m_cameraObject->GetComponent<Camera>();
	}
}