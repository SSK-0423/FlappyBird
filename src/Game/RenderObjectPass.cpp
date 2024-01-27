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
#ifdef _DEBUG
		//m_viewport = CD3DX12_VIEWPORT(
		//	0.f, 0.f, static_cast<float>(1024), static_cast<float>(768));
		//m_scissorRect = CD3DX12_RECT(0, 0, 1024, 768);
		m_viewport = CD3DX12_VIEWPORT(
			0.f, 0.f, static_cast<float>(1920), static_cast<float>(1080));
		m_scissorRect = CD3DX12_RECT(0, 0, 1920, 1080);
#else
		m_viewport = Dx12GraphicsEngine::GetViewport();
		m_scissorRect = Dx12GraphicsEngine::GetScissorRect();
#endif // _DEBUG

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

		auto renderTargetSize = Dx12GraphicsEngine::GetFrameRenderTargetSize();
		Editor::DebugLog("renderTargetSize %d %d", renderTargetSize.cx, renderTargetSize.cy);

		RenderTargetData data;
		data.depthStencilBufferData.width = renderTargetSize.cx;
		data.depthStencilBufferData.height = renderTargetSize.cy;

		data.renderTargetBufferData.clearColor[0] = 0.f;
		data.renderTargetBufferData.clearColor[1] = 0.f;
		data.renderTargetBufferData.clearColor[2] = 0.f;
		data.renderTargetBufferData.clearColor[3] = 1.f;
		data.renderTargetBufferData.colorFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		data.renderTargetBufferData.width = renderTargetSize.cx;
		data.renderTargetBufferData.height = renderTargetSize.cy;
		data.useDepth = true;	// 深度は一つでよいのでカラー出力のレンダーターゲットのみで使用する

		return m_renderTarget->Create(Dx12GraphicsEngine::Device(), data);
	}
}
