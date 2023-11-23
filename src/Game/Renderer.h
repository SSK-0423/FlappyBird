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
		void RenderScene(const std::list<std::shared_ptr<class Framework::Object>>& gameObjects);
		void RenderUI(const std::vector<std::unique_ptr<class Framework::Canvas>>& canvases);

	public:
		Utility::RESULT Init() override;
	};
}