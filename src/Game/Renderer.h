#pragma once
#include <d3d12.h>

#include "Framework/IRenderer.h"

namespace FlappyBird
{
	class Renderer : public Framework::IRenderer
	{
	public:
		Renderer();
		~Renderer();

	private:
		void RenderScene(const std::list<class Framework::GameObject*>& gameObjects);
		void RenderUI(const std::list<class Framework::UIObject*>& uiObjects);

	public:
		Utility::RESULT Init() override;
	};
}