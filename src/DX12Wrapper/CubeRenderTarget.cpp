#include "CubeRenderTarget.h"

#include "DescriptorHeapRTV.h"
#include "DescriptorHeapCBV_SRV_UAV.h"
#include "RenderTargetBuffer.h"

#include "DescriptorHeapDSV.h"
#include "DepthStencilBuffer.h"

#include "Texture.h"
#include "ShaderResourceViewDesc.h"

using namespace Utility;

namespace DX12Wrapper
{
	RESULT CubeRenderTarget::Create(ID3D12Device& device, CubeRenderTargetData& renderTargetData)
	{
		m_renderTargetData = renderTargetData;

		// �����_�[�^�[�Q�b�g����
		RESULT result = m_renderTargetBuffer.Create(device, renderTargetData.renderTargetBufferData);
		if (result == RESULT::FAILED) { return result; }

		// �����_�[�^�[�Q�b�g�q�[�v����
		result = m_rtvHeap.Create(device);
		if (result == RESULT::FAILED) { return result; }

		// �����_�[�^�[�Q�b�g�r���[����
		m_rtvHeap.RegistDescriptor(
			device, m_renderTargetBuffer, m_renderTargetData.renderTargetBufferData.colorFormat);

		// �I�t�X�N���[���e�N�X�`���o�b�t�@�[����
		m_renderTargetTexture.CreateTextureFromRenderTarget(m_renderTargetBuffer);

		// �e�N�X�`���p�q�[�v����
		result = m_textureHeap.Create(device);
		if (result == RESULT::FAILED) { return result; }

		// �e�N�X�`���Ƃ��ēo�^
		ShaderResourceViewDesc renderDesc(m_renderTargetTexture);
		m_textureHeap.RegistShaderResource(device, m_renderTargetTexture, renderDesc);

		// �f�v�X�X�e���V���o�b�t�@�[����
		result = m_depthStencilBuffer.Create(device, renderTargetData.depthStencilBufferData);
		if (result == RESULT::FAILED) { return result; }

		// �f�v�X�X�e���V���p�f�B�X�N���v�^�q�[�v����
		result = m_dsvHeap.Create(device);
		if (result == RESULT::FAILED) { return result; }

		// �f�v�X�X�e���V���r���[����
		m_dsvHeap.RegistDescriptor(device, m_depthStencilBuffer);

		// �f�v�X�X�e���V���e�N�X�`���o�b�t�@�[����
		m_depthStencilTexture.CreateTextureFromDepthStencil(m_depthStencilBuffer);

		// �e�N�X�`���Ƃ��ēo�^
		ShaderResourceViewDesc depthDesc(m_depthStencilTexture);
		m_textureHeap.RegistShaderResource(device, m_depthStencilTexture, depthDesc);

		return RESULT::SUCCESS;
	}

	void CubeRenderTarget::BeginRendering(RenderingContext& renderContext, CD3DX12_VIEWPORT& viewport, CD3DX12_RECT& scissorRect)
	{
		// �����_�[�^�[�Q�b�g�Ɉڍs
		renderContext.TransitionResourceState(
			m_renderTargetBuffer.GetBuffer(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET);

		// �����_�[�^�[�Q�b�g�Z�b�g
		auto rtvHandle = m_rtvHeap.GetCPUDescriptorHandleForHeapStart();

		if (m_renderTargetData.useDepth) {

			// �[�x�o�b�t�@�[
			auto dsvHandle = m_dsvHeap.GetCPUDescriptorHandleForHeapStart();

			renderContext.SetRenderTarget(&rtvHandle, &dsvHandle);

			// �f�v�X�X�e���V���o�b�t�@�[���N���A
			renderContext.ClearDepthStencilView(
				dsvHandle, D3D12_CLEAR_FLAG_DEPTH,
				m_renderTargetData.depthStencilBufferData.clearDepth,
				m_renderTargetData.depthStencilBufferData.clearStencil, 0, nullptr);
		}
		else {
			renderContext.SetRenderTarget(&rtvHandle, nullptr);
		}

		// ��ʂ��w��F�ŃN���A
		renderContext.ClearRenderTarget(rtvHandle, m_renderTargetData.renderTargetBufferData.clearColor, 0, nullptr);

		// �r���[�|�[�g�A�V�U�[��`�Z�b�g
		renderContext.SetViewport(viewport);
		renderContext.SetScissorRect(scissorRect);
	}

	void CubeRenderTarget::EndRendering(RenderingContext& renderContext)
	{
		// �s�N�Z���V�F�[�_�[���\�[�X�ֈڍs
		renderContext.TransitionResourceState(
			m_renderTargetBuffer.GetBuffer(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	}

	void CubeRenderTarget::BeginMultiRendering(
		CubeRenderTarget* renderTargets, const size_t& length, RenderingContext& renderContext,
		CD3DX12_VIEWPORT& viewport, CD3DX12_RECT& scissorRect)
	{
		// �n���h��
		D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandles = new D3D12_CPU_DESCRIPTOR_HANDLE[length];
		D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandle = nullptr;

		for (size_t idx = 0; idx < length; idx++)
		{
			// �����_�[�^�[�Q�b�g�Ɉڍs
			renderContext.TransitionResourceState(
				renderTargets[idx].m_renderTargetBuffer.GetBuffer(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET);

			rtvHandles[idx] = renderTargets[idx].m_rtvHeap.GetCPUDescriptorHandleForHeapStart();

			// ��ʂ��w��F�ŃN���A
			renderContext.ClearRenderTarget(
				rtvHandles[idx], renderTargets[idx].m_renderTargetData.renderTargetBufferData.clearColor, 0, nullptr);

			// �f�v�X�X�e���V����1�����Z�b�g�ł��Ȃ��̂ŁA�ŏ��Ɍ����������̂��g�p����
			if (renderTargets[idx].m_renderTargetData.useDepth && dsvHandle == nullptr) {
				dsvHandle = new D3D12_CPU_DESCRIPTOR_HANDLE();
				auto handle = renderTargets[idx].m_dsvHeap.GetCPUDescriptorHandleForHeapStart();
				// GetCPUDescriptorHandleForHeapStart�̖߂�l���l�n���Ȃ̂ŁA
				// �X�R�[�v�𔲂���ۂ�dsvHandle��null�ƂȂ�̂�h�����߂ɃR�s�[����
				memcpy_s(
					dsvHandle,
					sizeof(D3D12_CPU_DESCRIPTOR_HANDLE),
					&handle,
					sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));

				// �f�v�X�X�e���V���o�b�t�@�[���N���A
				renderContext.ClearDepthStencilView(
					*dsvHandle, D3D12_CLEAR_FLAG_DEPTH,
					renderTargets[idx].m_renderTargetData.depthStencilBufferData.clearDepth,
					renderTargets[idx].m_renderTargetData.depthStencilBufferData.clearStencil, 0, nullptr);
			}
		}

		// �����_�[�^�[�Q�b�g�Z�b�g
		renderContext.SetRenderTargets(length, rtvHandles, dsvHandle);

		// �r���[�|�[�g�A�V�U�[��`�Z�b�g
		renderContext.SetViewport(viewport);
		renderContext.SetScissorRect(scissorRect);
	}

	void CubeRenderTarget::EndMultiRendering(CubeRenderTarget* renderTargets, const size_t& length, RenderingContext& renderContext)
	{
		for (size_t idx = 0; idx < length; idx++)
		{
			// �s�N�Z���V�F�[�_�[���\�[�X�ֈڍs
			renderContext.TransitionResourceState(
				renderTargets[idx].m_renderTargetBuffer.GetBuffer(),
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		}
	}
}