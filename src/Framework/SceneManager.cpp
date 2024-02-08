#include "SceneManager.h"
#include "IRenderer.h"
#include "GameObjectManager.h"

namespace Framework
{
	// static変数の実体化
	const char* SceneManager::m_currentSceneName = "";
	const char* SceneManager::m_nextSceneName = nullptr;
	std::unordered_map<const char*, std::unique_ptr<Scene>> SceneManager::m_scenes;

	void SceneManager::Init()
	{
		// 一番最初に呼ばれるシーンの初期化
		m_scenes[m_currentSceneName]->Init();
		// 一番最初に呼ばれるシーンの開始
		m_scenes[m_currentSceneName]->Start();
	}

	void SceneManager::Start()
	{
		m_scenes[m_currentSceneName]->Start();
	}

	void SceneManager::Update(float deltaTime)
	{
		m_scenes[m_currentSceneName]->Update(deltaTime);
	}
	void SceneManager::Draw(IRenderer& renderer)
	{
		renderer.Render(m_scenes[m_currentSceneName].get());
	}
	void SceneManager::LateUpdate(float deltaTime)
	{
		m_scenes[m_currentSceneName]->LateUpdate(deltaTime);

		// 次シーンが設定されている場合はシーンを切り替える
		if (m_nextSceneName != nullptr)
		{
			const char* nextSceneName = m_nextSceneName;
			const char* currentSceneName = m_currentSceneName;
			int currentObjectCount = GameObjectManager::GetAllObject().size();

			// 現在のシーンの終了処理
			m_scenes[m_currentSceneName]->Final();

			// 次フレームから次シーンを開始できるようにここで初期化する
			m_currentSceneName = m_nextSceneName;
			m_scenes[m_currentSceneName]->Init();
			m_scenes[m_currentSceneName]->Start();
			m_nextSceneName = nullptr;

			Utility::DebugLog("Scene Change: %s -> %s\n", currentSceneName, nextSceneName);
			Utility::DebugLog("%s Object Count: %d\n", currentSceneName, currentObjectCount);
			Utility::DebugLog("%s Object Count: %d\n", currentSceneName, GameObjectManager::GetAllObject().size());
		}
	}
	void SceneManager::Final()
	{
		if (m_scenes[m_currentSceneName] != nullptr)
		{
			m_scenes[m_currentSceneName]->Final();
		}
	}
	void SceneManager::SetFirstScene(const char* name)
	{
		m_currentSceneName = name;
	}
	void SceneManager::SetNextScene(const char* name)
	{
		m_nextSceneName = name;
	}
	const std::unordered_map<const char*, std::unique_ptr<Scene>>& SceneManager::GetAllScene()
	{
		return m_scenes;
	}
}