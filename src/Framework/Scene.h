#pragma once
#include "Object.h"
#include "Canvas.h"
#include "Camera.h"

namespace Framework
{
	class Scene
	{
	public:
		Scene();
		virtual ~Scene() = default;

		virtual void Init() = 0;
		virtual void Update(float deltaTime);
		virtual void LateUpdate(float deltaTime);
		virtual void Final() = 0;

		const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const;
		const std::vector<std::unique_ptr<Canvas>>& GetCanvases() const;

		static const Camera& GetCamera();
	protected:
		std::vector<std::unique_ptr<GameObject>> m_gameObjects;
		std::vector<std::unique_ptr<Canvas>> m_canvases;
		static std::unique_ptr<GameObject> m_cameraObject;
	};
}