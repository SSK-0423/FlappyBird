#include "pch.h"
#include "IRenderer.h"
#include "Scene.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace DX12Wrapper;

namespace Framework
{
	void IRenderer::Render(const Scene* scene)
	{
		auto& graphicsEngine = DX12Wrapper::Dx12GraphicsEngine::Instance();

		graphicsEngine.BeginDraw();
		{
			RenderScene(scene->GetGameObjects());
			RenderUI(scene->GetCanvases());
		}
		graphicsEngine.EndDraw();
	}
}