#include "pch.h"
#include "RenderUIPass.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"
#include "DX12Wrapper/RenderingContext.h"
#include "DX12Wrapper/RootSignature.h"
#include "DX12Wrapper/GraphicsPipelineState.h"

using namespace DX12Wrapper;
using namespace Framework;
using namespace Utility;

namespace FlappyBird
{
	RenderUIPass::RenderUIPass()
	{
	}
	RenderUIPass::~RenderUIPass()
	{
	}
	RESULT RenderUIPass::Init()
	{
		// ビューポートとシザー矩形の設定
		auto windowSize = Window::GetWindowSize();
		m_viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, windowSize.cx, windowSize.cy);
		m_scissorRect = CD3DX12_RECT(0, 0, windowSize.cx, windowSize.cy);

		return RESULT::SUCCESS;
	}
	void RenderUIPass::Render(const std::list<Framework::UIObject*>& uiObjects)
	{
		for (auto& obj : uiObjects)
		{
			if (obj->GetActive())
			{
				obj->Draw();
			}
		}
	}
}
