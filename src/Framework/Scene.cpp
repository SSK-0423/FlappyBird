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
		// シーンがインスタンス化されるたびにカメラが生成されるのを防ぐ
		if (m_cameraObject == nullptr)
		{
			m_cameraObject = std::make_unique<GameObject>();
			m_cameraObject->AddComponent<Camera>(m_cameraObject.get());
		}
	}
	void Scene::Update(float deltaTime)
	{
		// オブジェクト全体の更新
		ObjectManager::Update(deltaTime);

		// コリジョン判定
		CollisionSystem::Update(deltaTime);

		// UIの更新
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