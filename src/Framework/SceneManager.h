#pragma once
#include "Scene.h"

namespace Framework
{
	class SceneManager
	{
	private:
		SceneManager() : m_activeSceneName("") {};
		~SceneManager() = default;
	public:

		static SceneManager& Instance()
		{
			static SceneManager inst;
			return inst;
		}

		void ActiveSceneUpdate(float deltaTime);

		void ActiveSceneDraw(class IRenderer& renderer);

		void ActiveSceneLateUpdate(float deltaTime);

		void ActiveSceneFinal();

		void LoadScene(const char* name);

		template<class T>
		void AddScene(const char* name)
		{
			m_scenes.insert(std::make_pair(name, std::make_unique<T>()));
		}

	private:
		const char* m_activeSceneName = "";
		const char* m_oldSceneName = "";
		bool m_isSceneChanged = false;
		std::unordered_map<const char*, std::unique_ptr<Scene>> m_scenes;
	};
}