#pragma once

#include "../Utility/EngineUtility.h"

#include "Texture.h"
#include "DescriptorHeapCBV_SRV_UAV.h"
#include "RenderTargetBuffer.h"
#include "DepthStencilBuffer.h"
#include "DescriptorHeapRTV.h"
#include "DescriptorHeapDSV.h"

namespace DX12Wrapper
{
	class RenderingContext;

	struct RenderTargetData
	{
		RenderTargetBufferData renderTargetBufferData;	// �����_�[�^�[�Q�b�g�o�b�t�@�[�p�f�[�^
		DepthStencilBufferData depthStencilBufferData;	// �f�v�X�X�e���V���o�b�t�@�[�p�f�[�^
		bool useDepth;
		bool isCubeMap;
	};

	/// <summary>
	/// �����_�[�^�[�Q�b�g�N���X
	/// </summary>
	class RenderTarget
	{
	private:
		Texture m_renderTargetTexture;				// �I�t�X�N���[���e�N�X�`��
		DescriptorHeapCBV_SRV_UAV m_textureHeap;		// �I�t�X�N���[���e�N�X�`���p�q�[�v

		RenderTargetBuffer m_renderTargetBuffer;		// �����_�[�^�[�Q�b�g�o�b�t�@�[
		DescriptorHeapRTV m_rtvHeap;					// �����_�[�^�[�Q�b�g�p�q�[�v

		Texture m_depthStencilTexture;				// �f�v�X�X�e���V���e�N�X�`��
		DepthStencilBuffer m_depthStencilBuffer;		// �f�v�X�X�e���V���o�b�t�@�[
		DescriptorHeapDSV m_dsvHeap;					// �f�v�X�X�e���V���p�q�[�v

		RenderTargetData m_renderTargetData;			// �����_�[�^�[�Q�b�g�f�[�^

	public:
		/// <summary>
		/// �����_�[�^�[�Q�b�g����
		/// </summary>
		/// <param name="device">�f�o�C�X</param>
		/// <returns></returns>
		Utility::RESULT Create(ID3D12Device& device, RenderTargetData& renderTargetData);

		/// <summary>
		/// �����_�����O�J�n
		/// </summary>
		/// <param name="renderContext">�����_�����O�R���e�L�X�g</param>
		void BeginRendering(RenderingContext& renderContext, CD3DX12_VIEWPORT& viewport, CD3DX12_RECT& scissorRect);

		/// <summary>
		/// �����_�����O�I��
		/// </summary>
		/// <param name="renderContext">�����_�����O�R���e�L�X�g</param>
		void EndRendering(RenderingContext& renderContext);

		/// <summary>
		/// �����_�[�^�[�Q�b�g�̃e�N�X�`���擾
		/// </summary>
		/// <returns>�����_�[�^�[�Q�b�g�e�N�X�`��</returns>
		Texture& GetRenderTargetTexture() {
			return m_renderTargetTexture;
		}

		/// <summary>
		/// �f�v�X�X�e���V���̃e�N�X�`���擾
		/// </summary>
		/// <returns>�f�v�X�X�e���V���e�N�X�`��</returns>
		Texture& GetDepthStencilTexture() {
			return m_depthStencilTexture;
		}

		/// <summary>
		/// �}���`�����_�[�^�[�Q�b�g�ł̃����_�����O�J�n
		/// </summary>
		/// <param name="renderTargets"></param>
		/// <param name="renderContext"></param>
		/// <param name="viewport"></param>
		/// <param name="scissorRect"></param>
		static void BeginMultiRendering(
			RenderTarget* renderTargets, const size_t& length, RenderingContext& renderContext,
			CD3DX12_VIEWPORT& viewport, CD3DX12_RECT& scissorRect);

		/// <summary>
		/// �}���`�����_�[�^�[�Q�b�g�ł̃����_�����O�I��
		/// </summary>
		/// <param name="renderTargets"></param>
		/// <param name="length"></param>
		/// <param name="renderContext"></param>
		static void EndMultiRendering(
			RenderTarget* renderTargets, const size_t& length, RenderingContext& renderContext);
	};
}