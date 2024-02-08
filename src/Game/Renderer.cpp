#include "pch.h"
#include "Renderer.h"
#include "Framework\ShaderLibrary.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"
#include "DX12Wrapper/RootSignature.h"
#include "DX12Wrapper/GraphicsPipelineState.h"
#include "DX12Wrapper/InputLayout.h"
#include "DX12Wrapper/ShaderResourceViewDesc.h"

#include "Framework/Scene.h"
#include "Framework/Object.h"

#include "RenderObjectPass.h"
#include "RenderScreenPass.h"
#include "RenderUIPass.h"

using namespace Utility;
using namespace DX12Wrapper;
using namespace Framework;

namespace FlappyBird
{
	Renderer::Renderer()
	{
	}
	Renderer::~Renderer()
	{
	}
	Utility::RESULT Renderer::Init()
	{
		// オブジェクト描画パスを初期化
		m_renderObjectPass = std::make_unique<RenderObjectPass>();
		if (m_renderObjectPass->Init() == RESULT::FAILED)
		{
			return RESULT::FAILED;
		}
		// UI描画パスを初期化
		m_renderUIPass = std::make_unique<RenderUIPass>();
		if (m_renderUIPass->Init() == RESULT::FAILED)
		{
			return RESULT::FAILED;
		}
		// スクリーン描画パスを初期化
		m_renderScreenPass = std::make_unique<RenderScreenPass>();
		if (m_renderScreenPass->Init() == RESULT::FAILED)
		{
			return RESULT::FAILED;
		}

		// オブジェクトの描画結果を渡す
		m_renderUIPass->SetRenderTexture(m_renderObjectPass->GetRenderTexture());
		//m_renderScreenPass->SetRenderTexture(m_renderUIPass->GetRenderTexture());
		m_renderScreenPass->SetRenderTexture(m_renderObjectPass->GetRenderTexture());

		return RESULT::SUCCESS;
	}
	void Renderer::RenderScene(const std::list<std::shared_ptr<GameObject>>& gameObjects)
	{
		// オブジェクト描画
		m_renderObjectPass->Render(gameObjects);
	}
	void Renderer::RenderUI(const std::list<std::shared_ptr<UIObject>>& uiObjects)
	{
		// UI描画パス
		m_renderUIPass->Render(uiObjects);
		// スクリーン描画パス
		m_renderScreenPass->Render();
	}
}
