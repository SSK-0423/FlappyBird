#pragma once
#include "Scene.h"

namespace Framework
{
	class SceneManager final
	{
	public:
		static void Init();

		static void Start();

		static void Update(float deltaTime);

		static void Draw(class IRenderer& renderer);

		static void LateUpdate(float deltaTime);

		static void Final();

		static void SetFirstScene(std::string name);

		static void SetNextScene(std::string name);

		static std::string GetCurrentSceneName();

		template<class T>
		static void RegistScene(std::string name)
		{
			m_scenes.insert(std::make_pair(name, std::make_unique<T>()));
		}

		static const std::unordered_map<std::string, std::unique_ptr<Scene>>& GetAllScene();

	private:
		SceneManager() = default;
		~SceneManager() = default;
		static std::string m_currentSceneName;
		static std::string m_nextSceneName;
		static std::unordered_map<std::string, std::unique_ptr<Scene>> m_scenes;
	};
}