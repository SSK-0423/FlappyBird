#include "pch.h"
#include "IRenderer.h"
#include "Scene.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace DX12Wrapper;

namespace Framework
{
	void IRenderer::Render(const Scene* scene)
	{
		Dx12GraphicsEngine::BeginDraw();
		{
			RenderScene(scene->GetGameObjects());
			RenderUI(scene->GetCanvases());
		}
		Dx12GraphicsEngine::EndDraw();
	}
}