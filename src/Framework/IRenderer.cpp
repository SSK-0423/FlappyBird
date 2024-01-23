#include "pch.h"
#include "IRenderer.h"
#include "Scene.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

#include "GameObjectManager.h"
#include "UIObjectManager.h"
#include "Editor.h"

using namespace DX12Wrapper;

namespace Framework
{
	void IRenderer::Render(const Scene* scene)
	{
		Dx12GraphicsEngine::BeginDraw();
		{
			Editor::BeginDraw();
			RenderScene(GameObjectManager::GetAllObject());
			RenderUI(UIObjectManager::GetAllObject());
			Editor::TestDraw();
			Editor::EndDraw();
		}
		Dx12GraphicsEngine::EndDraw();
	}
}