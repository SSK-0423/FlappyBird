#include "pch.h"
#include "ButtonRenderer.h"
#include "ShaderLibrary.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"
#include "DX12Wrapper/RootSignature.h"
#include "DX12Wrapper/GraphicsPipelineState.h"
#include "DX12Wrapper/InputLayout.h"

#include "Framework/Scene.h"

using namespace Utility;
using namespace DX12Wrapper;

namespace Framework
{
	ButtonRenderer::ButtonRenderer()
		: m_rootSignature(std::make_unique<RootSignature>()), m_pipelineState(std::make_unique<GraphicsPipelineState>())
	{
	}
	ButtonRenderer::~ButtonRenderer()
	{
	}
	Utility::RESULT ButtonRenderer::Init()
	{
		ID3D12Device& device = Dx12GraphicsEngine::Instance().Device();

		if (CreateRootSignature(device) == RESULT::FAILED)
		{
			MessageBoxA(NULL, "RootSignatureの生成に失敗", "エラー", MB_OK);
			return RESULT::FAILED;
		}
		if (CreateGraphicsPipelineState(device) == RESULT::FAILED)
		{
			MessageBoxA(NULL, "GraphicsPipelineStateの生成に失敗", "エラー", MB_OK);
			return RESULT::FAILED;
		}

		return RESULT::SUCCESS;
	}
	Utility::RESULT ButtonRenderer::CreateGraphicsPipelineState(ID3D12Device& device)
	{
		// ルートシグネチャとシェーダーセット
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineState = {};
		pipelineState.pRootSignature = &m_rootSignature->GetRootSignature();
		pipelineState.VS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::Instance().GetShader("ButtonVS")->GetShader());
		pipelineState.PS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::Instance().GetShader("ButtonPS")->GetShader());

		// サンプルマスク設定
		pipelineState.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		// ブレンド
		pipelineState.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		pipelineState.BlendState.AlphaToCoverageEnable = true;
		pipelineState.BlendState.RenderTarget[0].BlendEnable = true;
		pipelineState.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		pipelineState.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

		// ラスタライズ設定
		pipelineState.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineState.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;

		// インプットレイアウトの設定
		InputLayout layout;
		layout.Append("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0);
		layout.Append("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0);
		pipelineState.InputLayout.pInputElementDescs = layout.GetData();
		pipelineState.InputLayout.NumElements = layout.GetSize();
		pipelineState.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		pipelineState.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		// デプスステンシル設定
		pipelineState.DepthStencilState.DepthEnable = true;
		pipelineState.DepthStencilState.StencilEnable = false;
		pipelineState.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		pipelineState.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		pipelineState.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		// レンダーターゲットの設定
		pipelineState.NumRenderTargets = 1;
		pipelineState.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;	// カラー

		// アンチエイリアシングのためのサンプル数設定
		pipelineState.SampleDesc.Count = 1;
		pipelineState.SampleDesc.Quality = 0;

		return m_pipelineState->Create(device, pipelineState);
	}
	Utility::RESULT ButtonRenderer::CreateRootSignature(ID3D12Device& device)
	{
		RootSignatureData rootSigData;
		rootSigData._descRangeData.cbvDescriptorNum = 2;
		rootSigData._descRangeData.srvDescriptorNum = 1;

		return m_rootSignature->Create(device, rootSigData);
	}
	void ButtonRenderer::Render()
	{
	}
}