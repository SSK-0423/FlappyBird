#include "pch.h"
#include "RenderObjectPass.h"

#include "../DX12Wrapper/RenderTarget.h"
#include "../DX12Wrapper/RootSignature.h"
#include "../DX12Wrapper/GraphicsPipelineState.h"

using namespace Framework;
using namespace DX12Wrapper;
using namespace Utility;

namespace FlappyBird
{
	RenderObjectPass::RenderObjectPass()
	{
	}
	RenderObjectPass::~RenderObjectPass()
	{
	}
	RESULT RenderObjectPass::Init()
	{
		// レンダーターゲットの作成
		if (CreateRenderTarget(Dx12GraphicsEngine::Device()) == RESULT::FAILED)
		{
			return RESULT::FAILED;
		}
		// ビューポートとシザー矩形の設定
		auto windowSize = Window::GetWindowSize();
		m_viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, windowSize.cx, windowSize.cy);
		m_scissorRect = CD3DX12_RECT(0, 0, windowSize.cx, windowSize.cy);

		return RESULT::SUCCESS;
	}
	void RenderObjectPass::Render(const std::list<Framework::GameObject*>& gameObjects)
	{
		m_renderTarget->BeginRendering(Dx12GraphicsEngine::GetRenderingContext(), m_viewport, m_scissorRect);
		{
			for (auto& object : gameObjects)
			{
				if (object->GetActive())
				{
					object->Draw();
				}
			}
		}
		m_renderTarget->EndRendering(Dx12GraphicsEngine::GetRenderingContext());
	}
	DX12Wrapper::Texture& RenderObjectPass::GetRenderTexture()
	{
		return m_renderTarget->GetRenderTargetTexture();
	}
	RESULT RenderObjectPass::CreateRenderTarget(ID3D12Device& device)
	{
		m_renderTarget = std::make_unique<RenderTarget>();

		auto windowSize = Window::GetWindowSize();

		RenderTargetData data;
		data.depthStencilBufferData.width = windowSize.cx;
		data.depthStencilBufferData.height = windowSize.cy;

		data.renderTargetBufferData.clearColor[0] = 0.f;
		data.renderTargetBufferData.clearColor[1] = 0.f;
		data.renderTargetBufferData.clearColor[2] = 0.f;
		data.renderTargetBufferData.clearColor[3] = 1.f;
		data.renderTargetBufferData.colorFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		data.renderTargetBufferData.width = windowSize.cx;
		data.renderTargetBufferData.height = windowSize.cy;
		data.useDepth = true;	// 深度は一つでよいのでカラー出力のレンダーターゲットのみで使用する

		return m_renderTarget->Create(Dx12GraphicsEngine::Device(), data);
	}
}
