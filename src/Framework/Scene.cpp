#include "Scene.h"
#include "IRenderer.h"
#include "Transform2D.h"
#include "GameObjectManager.h"
#include "UIObjectManager.h"
#include "SoundManager.h"
#include "CollisionSystem.h"

namespace Framework
{
	// 静的メンバ変数の実体化
	std::shared_ptr<GameObject> Scene::m_cameraObject = nullptr;

	Scene::Scene() : m_fixedUpdateInterval(0.02f), m_elapsedTime(0.f)
	{
		// シーンがインスタンス化されるたびにカメラが生成されるのを防ぐ
		if (m_cameraObject == nullptr)
		{
			m_cameraObject = std::shared_ptr<GameObject>(new GameObject());
			m_cameraObject->AddComponent<Camera>(m_cameraObject);
		}
	}
	void Scene::Start()
	{
		// オブジェクト全体の開始
		GameObjectManager::Start();

		// UIの開始
		UIObjectManager::Start();
	}
	void Scene::Update(float deltaTime)
	{
		// オブジェクト全体の更新
		GameObjectManager::Update(deltaTime);

		// コリジョン判定
		CollisionSystem::Update(deltaTime);

		// UIの更新
		UIObjectManager::Update(deltaTime);

		// 一定間隔起きにFixedUpdateを呼び出す
		// 別スレッドで処理するべき？
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
		// 全てのゲームオブジェクトを削除
		GameObjectManager::Reset();

		// 全てのUIオブジェクトを削除
		UIObjectManager::Reset();

		// コリジョンシステムをリセット
		CollisionSystem::Reset();

		// サウンドをリセット
		SoundManager::Reset();
	}
	const Camera& Scene::GetCamera()
	{
		return *m_cameraObject->GetComponent<Camera>();
	}
}