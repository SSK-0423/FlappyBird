#pragma once
#include "Scene.h"

namespace Framework
{
	class SceneManager final
	{
	public:
		static void Init();

		static void Update(float deltaTime);

		static void Draw(class IRenderer& renderer);

		static void LateUpdate(float deltaTime);

		static void Final();

		static void SetFirstScene(const char* name);

		static void SetNextScene(const char* name);

		template<class T>
		static void RegistScene(const char* name)
		{
			m_scenes.insert(std::make_pair(name, std::make_unique<T>()));
		}

	private:
		SceneManager() = default;
		~SceneManager() = default;
		static const char* m_currentSceneName;
		static const char* m_nextSceneName;
		static std::unordered_map<const char*, std::unique_ptr<Scene>> m_scenes;
	};
}