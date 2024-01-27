#include "pch.h"
#include "RenderUIPass.h"
#include "FlappyBird.h"

#include "../DX12Wrapper/RenderTarget.h"
#include "../DX12Wrapper/RootSignature.h"
#include "../DX12Wrapper/GraphicsPipelineState.h"

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
			0.f, 0.f, static_cast<float>(1024), static_cast<float>(768));
		m_scissorRect = CD3DX12_RECT(0, 0, 1920, 1080);
#else
		m_viewport = Dx12GraphicsEngine::GetViewport();
		m_scissorRect = Dx12GraphicsEngine::GetScissorRect();
#endif // _DEBUG

		// オブジェクトのレンダリング結果を背景として表示するスプライト
		UIObject* objectRenderSprite = UIObjectManager::CreateObject();
		objectRenderSprite->SetName("RenderSprite");

		SpriteRenderer* spriteRenderer = objectRenderSprite->AddComponent<SpriteRenderer>(objectRenderSprite);
		spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GUI);
		spriteRenderer->SetLayer(static_cast<UINT>(SPRITE_LAYER::BACKGROUND));

		Transform2D* transform = objectRenderSprite->GetComponent<Transform2D>();
		transform->position = { m_viewport.Width / 2.f, m_viewport.Height / 2.f };
		transform->scale = { m_viewport.Width * 1.f, m_viewport.Height * 1.f };

		return RESULT::SUCCESS;
	}
	void RenderUIPass::Render(const std::list<Framework::UIObject*>& uiObjects)
	{
		//m_renderTarget->BeginRendering(Dx12GraphicsEngine::GetRenderingContext(), m_viewport, m_scissorRect);
		{
			for (auto& object : uiObjects)
			{
				if (object->GetActive())
				{
					object->Draw();
				}
			}
		}
		//m_renderTarget->EndRendering(Dx12GraphicsEngine::GetRenderingContext());
	}
	void RenderUIPass::SetRenderTexture(DX12Wrapper::Texture& texture)
	{
		// RenderTargetのテクスチャはスタック領域にあるので、
		Sprite* sprite = new Sprite(L"");
		std::shared_ptr<Texture> texturePtr = std::make_shared<Texture>(texture);
		sprite->SetTexture(texturePtr);
	}
	DX12Wrapper::Texture& RenderUIPass::GetRenderTexture()
	{
		return m_renderTarget->GetRenderTargetTexture();
	}
	Utility::RESULT RenderUIPass::CreateRenderTarget(ID3D12Device& device)
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
