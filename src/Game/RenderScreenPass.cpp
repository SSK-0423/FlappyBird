#include "pch.h"
#include "RenderScreenPass.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"
#include "DX12Wrapper/RenderingContext.h"
#include "DX12Wrapper/RootSignature.h"
#include "DX12Wrapper/GraphicsPipelineState.h"

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

		// ビューポートとシザー矩形の設定
		auto windowSize = Window::GetWindowSize();
		m_viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, windowSize.cx, windowSize.cy);
		m_scissorRect = CD3DX12_RECT(0, 0, windowSize.cx, windowSize.cy);

		return RESULT::SUCCESS;
	}
	void RenderScreenPass::Render()
	{
		Dx12GraphicsEngine::SetFrameRenderTarget(m_viewport, m_scissorRect);
		{
			RenderingContext renderingContext = Dx12GraphicsEngine::GetRenderingContext();

			renderingContext.SetGraphicsRootSignature(*m_rootSignature);
			renderingContext.SetPipelineState(*m_pipelineState);
			renderingContext.SetDescriptorHeap(*m_descriptorHeap);
		}
	}
	void RenderScreenPass::SetRenderTexture(ShaderResourceViewDesc& desc, Texture& texture)
	{
		m_descriptorHeap->RegistShaderResource(Dx12GraphicsEngine::Device(), texture, desc, 0);
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
}