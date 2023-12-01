#pragma once

#include "Utility/EngineUtility.h"

namespace Framework
{
	class IRenderer
	{
	public:
		IRenderer() = default;
		virtual ~IRenderer() = default;
		virtual Utility::RESULT Init() = 0;
		void Render(const class Scene* scene);

	private:
		virtual void RenderScene(const std::list<class GameObject*>& gameObjects) = 0;
		virtual void RenderUI(const std::list<class UIObject*>& uiObjects) = 0;
	};
}