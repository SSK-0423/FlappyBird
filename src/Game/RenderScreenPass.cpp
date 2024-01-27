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
		// ���[�g�V�O�l�`������
		if (CreateRootSignature() == RESULT::FAILED)
		{
			return RESULT::FAILED;
		}
		// �p�C�v���C���X�e�[�g����
		if (CreatePipelineState() == RESULT::FAILED)
		{
			return RESULT::FAILED;
		}
		// �f�B�X�N���v�^�q�[�v����
		if (CreateDescriptorHeap() == RESULT::FAILED)
		{
			return RESULT::FAILED;
		}
		// �����_�[�^�[�Q�b�g����
		if (CreateRenderTarget() == RESULT::FAILED)
		{
			return RESULT::FAILED;
		}

		// �r���[�|�[�g�ƃV�U�[��`�̐ݒ�
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
		//			// �t���X�N���[���|���S���̒��_�Z�b�g
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
		//			// �t���X�N���[���|���S���̒��_�Z�b�g
		//			renderingContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//			renderingContext.DrawInstanced(4, 1, 0, 0);
		//		}
		//#endif

		Dx12GraphicsEngine::SetFrameRenderTarget(m_viewport, m_scissorRect);
		{
			renderingContext.SetGraphicsRootSignature(*m_rootSignature);
			renderingContext.SetPipelineState(*m_pipelineState);
			renderingContext.SetDescriptorHeap(*m_descriptorHeap);
			// �t���X�N���[���|���S���̒��_�Z�b�g
			renderingContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			renderingContext.DrawInstanced(4, 1, 0, 0);
		}
	}
	void RenderScreenPass::SetRenderTexture(Texture& texture)
	{
		ShaderResourceViewDesc desc(texture);
		m_descriptorHeap->RegistShaderResource(Dx12GraphicsEngine::Device(), texture, desc, 0);
#ifdef _DEBUG
		// �f�o�b�O���[�h����ImGui�̃f�B�X�N���v�^�q�[�v�ɂ��o�^
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

		// ���[�g�V�O�l�`���ƃV�F�[�_�[�Z�b�g
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineState = {};
		pipelineState.pRootSignature = &m_rootSignature->GetRootSignature();
		pipelineState.VS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::GetShader("FullScreenVS")->GetShader());
		pipelineState.PS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::GetShader("FullScreenPS")->GetShader());

		// �T���v���}�X�N�ݒ�
		pipelineState.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		// �u�����h
		pipelineState.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

		// ���X�^���C�Y�ݒ�
		pipelineState.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineState.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		// �C���v�b�g���C�A�E�g�̐ݒ�
		pipelineState.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		pipelineState.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		// �f�v�X�X�e���V���ݒ�
		pipelineState.DepthStencilState.DepthEnable = false;
		pipelineState.DepthStencilState.StencilEnable = false;

		// �����_�[�^�[�Q�b�g�̐ݒ�
		pipelineState.NumRenderTargets = 1;
		pipelineState.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

		// �A���`�G�C���A�V���O�̂��߂̃T���v�����ݒ�
		pipelineState.SampleDesc.Count = 1;			// �T���v�����O��1�s�N�Z���ɂ�1
		pipelineState.SampleDesc.Quality = 0;		// �N�I���e�B�͍Œ�

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