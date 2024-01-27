#include "pch.h"
#include "RenderScreenPass.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"
#include "DX12Wrapper/RenderingContext.h"
#include "DX12Wrapper/RootSignature.h"
#include "DX12Wrapper/GraphicsPipelineState.h"
#include "DX12Wrapper/RenderTarget.h"
#include "DX12Wrapper/ShaderResourceViewDesc.h"

using namespace DX12Wrapper;
using namespace Framework;
using namespace Utility;

namespace FlappyBird
{
	RenderScreenPass::RenderScreenPass()
	{
	}
	RenderScreenPass::~RenderScreenPass()
	{
	}
	RESULT RenderScreenPass::Init()
	{
		// ルートシグネチャ生成
		if (CreateRootSignature() == RESULT::FAILED)
		{
			return RESULT::FAILED;
		}
		// パイプラインステート生成
		if (CreatePipelineState() == RESULT::FAILED)
		{
			return RESULT::FAILED;
		}
		// ディスクリプタヒープ生成
		if (CreateDescriptorHeap() == RESULT::FAILED)
		{
			return RESULT::FAILED;
		}
		// レンダーターゲット生成
		if (CreateRenderTarget() == RESULT::FAILED)
		{
			return RESULT::FAILED;
		}

		// ビューポートとシザー矩形の設定
		m_viewport = Dx12GraphicsEngine::GetViewport();
		m_scissorRect = Dx12GraphicsEngine::GetScissorRect();

		return RESULT::SUCCESS;
	}
	void RenderScreenPass::Render()
	{
		RenderingContext renderingContext = Dx12GraphicsEngine::GetRenderingContext();

		//#ifdef _DEBUG
		//		m_renderTarget->BeginRendering(renderingContext, m_viewport, m_scissorRect);
		//		{
		//			renderingContext.SetGraphicsRootSignature(*m_rootSignature);
		//			renderingContext.SetPipelineState(*m_pipelineState);
		//			renderingContext.SetDescriptorHeap(*m_descriptorHeap);
		//			// フルスクリーンポリゴンの頂点セット
		//			renderingContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//			renderingContext.DrawInstanced(4, 1, 0, 0);
		//		}
		//		m_renderTarget->EndRendering(renderingContext);
		//
		//		Dx12GraphicsEngine::SetFrameRenderTarget(m_viewport, m_scissorRect);
		//#else
		//		Dx12GraphicsEngine::SetFrameRenderTarget(m_viewport, m_scissorRect);
		//		{
		//			renderingContext.SetGraphicsRootSignature(*m_rootSignature);
		//			renderingContext.SetPipelineState(*m_pipelineState);
		//			renderingContext.SetDescriptorHeap(*m_descriptorHeap);
		//			// フルスクリーンポリゴンの頂点セット
		//			renderingContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//			renderingContext.DrawInstanced(4, 1, 0, 0);
		//		}
		//#endif

		Dx12GraphicsEngine::SetFrameRenderTarget(m_viewport, m_scissorRect);
		{
			renderingContext.SetGraphicsRootSignature(*m_rootSignature);
			renderingContext.SetPipelineState(*m_pipelineState);
			renderingContext.SetDescriptorHeap(*m_descriptorHeap);
			// フルスクリーンポリゴンの頂点セット
			renderingContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			renderingContext.DrawInstanced(4, 1, 0, 0);
		}
	}
	void RenderScreenPass::SetRenderTexture(Texture& texture)
	{
		ShaderResourceViewDesc desc(texture);
		m_descriptorHeap->RegistShaderResource(Dx12GraphicsEngine::Device(), texture, desc, 0);
#ifdef _DEBUG
		// デバッグモード時はImGuiのディスクリプタヒープにも登録
		Editor::GetImGuiHeap().RegistShaderResource(Dx12GraphicsEngine::Device(), texture, desc, 0);
#endif // _DEBUG
	}
	RESULT RenderScreenPass::CreateRootSignature()
	{
		m_rootSignature = std::make_unique<RootSignature>();

		RootSignatureData rootSigData;
		rootSigData.m_descRangeData.srvDescriptorNum = 1;
		rootSigData.m_samplerData.samplerFilter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;

		if (m_rootSignature->Create(Dx12GraphicsEngine::Device(), rootSigData) == RESULT::FAILED)
		{
			return RESULT::FAILED;
		}
		return RESULT::SUCCESS;
	}
	RESULT RenderScreenPass::CreatePipelineState()
	{
		m_pipelineState = std::make_unique<GraphicsPipelineState>();

		// ルートシグネチャとシェーダーセット
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineState = {};
		pipelineState.pRootSignature = &m_rootSignature->GetRootSignature();
		pipelineState.VS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::GetShader("FullScreenVS")->GetShader());
		pipelineState.PS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::GetShader("FullScreenPS")->GetShader());

		// サンプルマスク設定
		pipelineState.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		// ブレンド
		pipelineState.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

		// ラスタライズ設定
		pipelineState.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineState.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		// インプットレイアウトの設定
		pipelineState.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		pipelineState.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		// デプスステンシル設定
		pipelineState.DepthStencilState.DepthEnable = false;
		pipelineState.DepthStencilState.StencilEnable = false;

		// レンダーターゲットの設定
		pipelineState.NumRenderTargets = 1;
		pipelineState.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

		// アンチエイリアシングのためのサンプル数設定
		pipelineState.SampleDesc.Count = 1;			// サンプリングは1ピクセルにつき1
		pipelineState.SampleDesc.Quality = 0;		// クオリティは最低

		return m_pipelineState->Create(Dx12GraphicsEngine::Device(), pipelineState);
	}
	Utility::RESULT RenderScreenPass::CreateDescriptorHeap()
	{
		m_descriptorHeap = std::make_unique<DescriptorHeapCBV_SRV_UAV>();

		return m_descriptorHeap->Create(Dx12GraphicsEngine::Device());
	}
	Utility::RESULT RenderScreenPass::CreateRenderTarget()
	{
		m_renderTarget = std::make_unique<RenderTarget>();

		auto renderTargetSize = Dx12GraphicsEngine::GetFrameRenderTargetSize();

		RenderTargetData data;
		data.renderTargetBufferData.width = renderTargetSize.cx;
		data.renderTargetBufferData.height = renderTargetSize.cy;
		data.renderTargetBufferData.colorFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		data.depthStencilBufferData.width = renderTargetSize.cx;
		data.depthStencilBufferData.height = renderTargetSize.cy;
		data.useDepth = false;

		return m_renderTarget->Create(Dx12GraphicsEngine::Device(), data);
	}
}