#include "SceneManager.h"
#include "IRenderer.h"

namespace Framework
{
	void SceneManager::ActiveSceneUpdate(float deltaTime)
	{
		m_scenes[m_activeSceneName]->Update(deltaTime);
	}
	void SceneManager::ActiveSceneDraw(IRenderer& renderer)
	{
		renderer.Render(m_scenes[m_activeSceneName].get());
	}
	void SceneManager::ActiveSceneLateUpdate(float deltaTime)
	{
		// シーンが切り替わった場合は前のシーンのLateUpdateを呼ぶ
		if (m_isSceneChanged && m_oldSceneName != "")
		{
			m_scenes[m_oldSceneName]->LateUpdate(deltaTime);
			m_isSceneChanged = false;
		}
		else
		{
			m_scenes[m_activeSceneName]->LateUpdate(deltaTime);
		}
	}
	void SceneManager::ActiveSceneFinal()
	{
		if (m_scenes[m_activeSceneName] != nullptr)
		{
			m_scenes[m_activeSceneName]->Final();
		}
	}
	void SceneManager::LoadScene(const char* name)
	{
		if (m_scenes[m_activeSceneName] != nullptr)
		{
			m_scenes[m_activeSceneName]->SetActive(false);
		}
		m_oldSceneName = m_activeSceneName;
		m_activeSceneName = name;
		m_scenes[m_activeSceneName]->SetActive(true);
		m_scenes[m_activeSceneName]->Init();
		m_isSceneChanged = true;
	}
}