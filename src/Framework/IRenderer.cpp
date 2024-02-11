#include "pch.h"
#include "IRenderer.h"
#include "Scene.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

#include "GameObjectManager.h"
#include "UIObjectManager.h"
#include "SoundManager.h"
#include "Editor.h"

using namespace DX12Wrapper;

namespace Framework
{
	void IRenderer::Render(const Scene* scene)
	{
		Dx12GraphicsEngine::BeginDraw();
		{
			Editor::BeginDraw();

			// ゲーム内で動作するエディタでImGuiを使うためにBeginDraw()とEndDraw()の間に記述
			RenderScene(GameObjectManager::GetAllObject());
			RenderUI(UIObjectManager::GetAllObject());

			// デバッグログなどのエディタ描画 
#ifdef _DEBUG
			Editor::Draw();
			SoundManager::DebugDraw();
#endif

			Editor::EndDraw();
		}
		Dx12GraphicsEngine::EndDraw();
	}
}