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
		virtual void RenderScene(const std::list<std::shared_ptr<class Object>>& gameObjects) = 0;
		virtual void RenderUI(const std::vector<std::unique_ptr<class Canvas>>& canvases) = 0;
	};
}