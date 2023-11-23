#include "SceneManager.h"
#include "IRenderer.h"

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
			// ���݂̃V�[���̏I������
			m_scenes[m_currentSceneName]->Final();
			
			// ���t���[�����玟�V�[�����J�n�ł���悤�ɂ����ŏ���������
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