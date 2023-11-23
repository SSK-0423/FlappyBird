#include "pch.h"
#include "IRenderer.h"
#include "Scene.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

#include "ObjectManager.h"

using namespace DX12Wrapper;

namespace Framework
{
	void IRenderer::Render(const Scene* scene)
	{
		Dx12GraphicsEngine::BeginDraw();
		{
			RenderScene(ObjectManager::GetAllObject());
			RenderUI(scene->GetCanvases());
		}
		Dx12GraphicsEngine::EndDraw();
	}
}