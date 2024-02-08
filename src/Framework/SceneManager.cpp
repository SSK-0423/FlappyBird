#include "SceneManager.h"
#include "IRenderer.h"
#include "GameObjectManager.h"

namespace Framework
{
	// static�ϐ��̎��̉�
	const char* SceneManager::m_currentSceneName = "";
	const char* SceneManager::m_nextSceneName = nullptr;
	std::unordered_map<const char*, std::unique_ptr<Scene>> SceneManager::m_scenes;

	void SceneManager::Init()
	{
		// ��ԍŏ��ɌĂ΂��V�[���̏�����
		m_scenes[m_currentSceneName]->Init();
		// ��ԍŏ��ɌĂ΂��V�[���̊J�n
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

		// ���V�[�����ݒ肳��Ă���ꍇ�̓V�[����؂�ւ���
		if (m_nextSceneName != nullptr)
		{
			const char* nextSceneName = m_nextSceneName;
			const char* currentSceneName = m_currentSceneName;
			int currentObjectCount = GameObjectManager::GetAllObject().size();

			// ���݂̃V�[���̏I������
			m_scenes[m_currentSceneName]->Final();

			// ���t���[�����玟�V�[�����J�n�ł���悤�ɂ����ŏ���������
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