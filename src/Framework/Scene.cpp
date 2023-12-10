#include "Scene.h"
#include "IRenderer.h"
#include "Transform2D.h"
#include "GameObjectManager.h"
#include "UIObjectManager.h"
#include "CollisionSystem.h"

namespace Framework
{
	// 静的メンバ変数の実体化
	std::shared_ptr<GameObject> Scene::m_cameraObject = nullptr;

	Scene::Scene()
	{
		// シーンがインスタンス化されるたびにカメラが生成されるのを防ぐ
		if (m_cameraObject == nullptr)
		{
			m_cameraObject = std::make_shared<GameObject>();
			m_cameraObject->AddComponent<Camera>(m_cameraObject.get());
		}
	}
	void Scene::Update(float deltaTime)
	{
		// オブジェクト全体の更新
		GameObjectManager::Update(deltaTime);

		// コリジョン判定
		CollisionSystem::Update(deltaTime);

		// UIの更新
		UIObjectManager::Update(deltaTime);

	}
	void Scene::LateUpdate(float deltaTime)
	{
	}
	void Scene::Final()
	{
		// 全てのゲームオブジェクトを削除
		GameObjectManager::Reset();

		// 全てのUIオブジェクトを削除
		UIObjectManager::Reset();

		// コリジョンシステムをリセット
		CollisionSystem::Reset();
	}
	const Camera& Scene::GetCamera()
	{
		return *m_cameraObject->GetComponent<Camera>();
	}
}