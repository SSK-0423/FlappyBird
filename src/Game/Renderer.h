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

		Utility::RESULT Init() override;
	private:
		void RenderScene(const std::list<std::shared_ptr<class Framework::GameObject>>& gameObjects) override;
		void RenderUI(const std::list<std::shared_ptr<class Framework::UIObject>>& uiObjects) override;

		std::unique_ptr<class RenderObjectPass> m_renderObjectPass = nullptr;
		std::unique_ptr<class RenderScreenPass> m_renderScreenPass = nullptr;
		std::unique_ptr<class RenderUIPass> m_renderUIPass = nullptr;
	};
}