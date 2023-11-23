#include "SceneManager.h"
#include "IRenderer.h"

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
			// 現在のシーンの終了処理
			m_scenes[m_currentSceneName]->Final();
			
			// 次フレームから次シーンを開始できるようにここで初期化する
			m_currentSceneName = m_nextSceneName;
			m_scenes[m_currentSceneName]->Init();
			m_nextSceneName = nullptr;
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
}