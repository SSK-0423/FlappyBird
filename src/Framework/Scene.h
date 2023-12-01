#pragma once
#include "Object.h"
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
		virtual void Final();

		static const Camera& GetCamera();
	protected:
		static std::shared_ptr<GameObject> m_cameraObject;
	};
}