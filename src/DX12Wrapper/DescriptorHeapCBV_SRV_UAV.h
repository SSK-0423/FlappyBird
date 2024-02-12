#pragma once

#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "../Utility/EngineUtility.h"


namespace DX12Wrapper
{
	class Texture;
	class ConstantBuffer;
	class UnorderedAccessResource;

	struct ShaderResourceViewDesc;

	/// <summary>
	/// �R���X�^���g�o�b�t�@�[�A�V�F�[�_�[���\�[�X�A�A���I�[�_�[�A�N�Z�X���\�[�X��
	/// �f�B�X�N���v�^�q�[�v�̃��b�p�[�N���X
	/// </summary>
	class DescriptorHeapCBV_SRV_UAV
	{
	public:
		// Sprite��DescriptorHeap�̃f�X�g���N�^�ŃG���[�o�Ă���ۂ�
		DescriptorHeapCBV_SRV_UAV() = default;
		~DescriptorHeapCBV_SRV_UAV() = default;
		static constexpr int NEXT_REGISTER = -1;

		/// <summary>
		/// �f�B�X�N���v�^�q�[�v����
		/// </summary>
		/// <param name="device">�f�o�C�X</param>
		/// <returns></returns>
		Utility::RESULT Create(ID3D12Device& device);

		/// <summary>
		/// GPU�̃f�B�X�N���v�^�q�[�v�̐擪�n���h�����擾
		/// </summary>
		/// <returns></returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStart() {
			return m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();
		}

		/// <summary>
		/// CPU�̃f�B�X�N���v�^�q�[�v�̐擪�n���h���擾
		/// </summary>
		/// <returns></returns>
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForHeapStart() {
			return m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
		}

		/// <summary>
		/// GPU��CBV�����̃f�B�X�N���v�^�q�[�v�̐擪�n���h�����擾
		/// </summary>
		/// <returns>GPU��CBV�����̃f�B�X�N���v�^�q�[�v�̐擪�n���h��</returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStartCBV() {
			return m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();
		}

		/// <summary>
		/// GPU��SRV�����̃f�B�X�N���v�^�q�[�v�̐擪�n���h�����擾
		/// </summary>
		/// <returns>GPU��SRV�����̃f�B�X�N���v�^�q�[�v�̐擪�n���h��</returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStartSRV() {
			auto gpuHandle = m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();
			gpuHandle.ptr += m_handleIncrimentSize * m_MAX_CBV_DESCRIPTOR_NUM;
			auto cpuHandle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
			cpuHandle.ptr += m_handleIncrimentSize * m_MAX_CBV_DESCRIPTOR_NUM;

			return std::move(gpuHandle);
		}

		/// <summary>
		/// �w��C���f�b�N�X��SRV�n���h�����擾
		/// </summary>
		/// <param name="index">�C���f�b�N�X</param>
		/// <returns>�w��C���f�b�N�X��SRV�n���h��</returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandle(int index) {
			auto gpuHandle = m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();
			gpuHandle.ptr += m_handleIncrimentSize * m_MAX_CBV_DESCRIPTOR_NUM;
			gpuHandle.ptr += m_handleIncrimentSize * index;
			auto cpuHandle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
			cpuHandle.ptr += m_handleIncrimentSize * m_MAX_CBV_DESCRIPTOR_NUM;
			cpuHandle.ptr += m_handleIncrimentSize * index;

			return std::move(gpuHandle);
		}

		/// <summary>
		/// GPU�n���h���擾
		/// </summary>
		/// <returns>GPU�n���h��</returns>
		CD3DX12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() {
			auto gpuHandle = m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();
			gpuHandle.ptr += m_handleIncrimentSize * m_MAX_CBV_DESCRIPTOR_NUM;
			auto cpuHandle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
			cpuHandle.ptr += m_handleIncrimentSize * m_MAX_CBV_DESCRIPTOR_NUM;

			return CD3DX12_GPU_DESCRIPTOR_HANDLE(gpuHandle, m_MAX_CBV_DESCRIPTOR_NUM, static_cast<UINT>(m_handleIncrimentSize));
		}

		/// <summary>
		/// GPU��UAV�����̃f�B�X�N���v�^�q�[�v�̐擪�n���h�����擾
		/// </summary>
		/// <returns>GPU��UAV�����̃f�B�X�N���v�^�q�[�v�̐擪�n���h��</returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStartUAV() {
			auto handle = m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();
			handle.ptr += m_handleIncrimentSize * (m_MAX_CBV_DESCRIPTOR_NUM + m_MAX_UAV_DESCRIPTOR_NUM);

			return handle;
		}

		/// <summary>
		/// �f�B�X�N���v�^�q�[�v�n���h���̃C���N�������g�T�C�Y�擾
		/// </summary>
		/// <returns>�C���N�������g�T�C�Y</returns>
		SIZE_T GetHandleIncrimentSize() {
			return m_handleIncrimentSize;
		}

		/// <summary>
		/// �R���X�^���g�o�b�t�@�\�o�^
		/// </summary>
		/// <param name="device">�f�o�C�X</param>
		/// <param name="constantBuffer">�R���X�^���g�o�b�t�@�\</param>
		/// <param name="registerNo">
		/// �o�^���郌�W�X�^�ԍ� �f�t�H���g�l:NEXT_REGISTER(-1) ���ݓo�^����Ă��郊�\�[�X���̎��̃��W�X�^���g�p
		/// </param>
		void RegistConstantBuffer(
			ID3D12Device& device, ConstantBuffer& constantBuffer, const int& registerNo = NEXT_REGISTER);

		/// <summary>
		/// �V�F�[�_�[���\�[�X(�e�N�X�`��)�o�^
		/// </summary>
		/// <param name="device">�f�o�C�X</param>
		/// <param name="texture">�e�N�X�`��</param>
		/// <param name="registerNo">
		/// �o�^���郌�W�X�^�ԍ� �f�t�H���g�l:NEXT_REGISTER(-1) ���ݓo�^����Ă��郊�\�[�X���̎��̃��W�X�^���g�p
		/// </param>
		int RegistShaderResource(
			ID3D12Device& device, Texture& texture, ShaderResourceViewDesc& desc, const int& registerNo = NEXT_REGISTER);

		/// <summary>
		/// �A���I�[�_�[�A�N�Z�X���\�[�X�o�^
		/// </summary>
		/// <param name="device">�f�o�C�X</param>
		/// <param name="unorderedAccessResource">�A���I�[�_�[�A�N�Z�X���\�[�X</param>
		/// <param name="registerNo">
		/// �o�^���郌�W�X�^�ԍ� �f�t�H���g�l:NEXT_REGISTER(-1) ���ݓo�^����Ă��郊�\�[�X���̎��̃��W�X�^���g�p
		/// </param>
		void RegistUnorderedAccessResource(
			ID3D12Device& device, UnorderedAccessResource& unorderedAccessResource,
			const int& registerNo = NEXT_REGISTER);

		/// <summary>
		/// �R���X�^���g�o�b�t�@�[���o�^����Ă��邩
		/// </summary>
		/// <returns></returns>
		bool IsRegistedConstantBuffer() { return m_registedCBVNum > 0; }

		/// <summary>
		/// �V�F�[�_�[���\�[�X���o�^����Ă��邩
		/// </summary>
		/// <returns></returns>
		bool IsRegistedShaderResource() { return m_registedSRVNum > 0; }

		/// <summary>
		/// �A���I�[�_�[�A�N�Z�X���\�[�X���o�^����Ă��邩
		/// </summary>
		/// <returns></returns>
		bool IsRegistedUnorderedAccessResource() { return m_registedUAVNum > 0; }

		/// <summary>
		/// �f�B�X�N���v�^�q�[�v�̃A�h���X�擾
		/// </summary>
		/// <returns>�f�B�X�N���v�^�q�[�v�̃A�h���X</returns>
		ID3D12DescriptorHeap** GetDescriptorHeapAddress() {
			return m_descriptorHeap.GetAddressOf();
		}

	private:
		static constexpr UINT m_MAX_CBV_DESCRIPTOR_NUM = 64;	// �萔�o�b�t�@�[�̍ő吔
		static constexpr UINT m_MAX_SRV_DESCRIPTOR_NUM = 64;	// �V�F�[�_�[���\�[�X�̍ő吔
		static constexpr UINT m_MAX_UAV_DESCRIPTOR_NUM = 64;	// �A���I�[�_�[�A�N�Z�X�̍ő吔

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_descriptorHeap = nullptr;

		SIZE_T m_handleIncrimentSize = 0;
		UINT m_registedCBVNum = 0;
		UINT m_registedSRVNum = 0;
		UINT m_registedUAVNum = 0;

		/// <summary>
		/// �f�B�X�N���v�^�q�[�v����
		/// </summary>
		/// <param name="device">�f�o�C�X</param>
		/// <returns></returns>
		HRESULT CreateDescriptorHeap(ID3D12Device& device);
	};
}