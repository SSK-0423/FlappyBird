#include "Scene.h"
#include "IRenderer.h"
#include "Transform2D.h"

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
		for (auto& obj : m_gameObjects)
		{
			if (obj->GetActive())
			{
				obj->Update(deltaTime);
			}
		}

		for (auto& canvas : m_canvases)
		{
			canvas->Update(deltaTime);
		}
	}
	void Scene::LateUpdate(float deltaTime)
	{
		if (!m_isActive)
		{
			Final();
		}
	}
	const std::vector<std::unique_ptr<GameObject>>& Scene::GetGameObjects() const
	{
		return m_gameObjects;
	}
	const std::vector<std::unique_ptr<Canvas>>& Scene::GetCanvases() const
	{
		return m_canvases;
	}
	void Scene::SetActive(bool isActive)
	{
		m_isActive = isActive;
	}
	const Camera& Scene::GetCamera()
	{
		return *m_cameraObject->GetComponent<Camera>();
	}
}