#pragma once
#include <d3d12.h>

#pragma comment(lib,"d3d12.lib")

#include "../Utility/EngineUtility.h"

namespace DX12Wrapper
{
	/// <summary>
	/// �R���X�^���g�o�b�t�@�\�N���X
	/// </summary>
	class ConstantBuffer {
	public:
		ConstantBuffer() = default;
		~ConstantBuffer() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> m_constantBuffer = nullptr;	// �o�b�t�@�[
		BYTE* m_mappedData = nullptr;	                                    // �f�[�^�̃}�b�v��
		UINT m_bufferSize = 0;	                                            // �o�b�t�@�[�T�C�Y

		/// <summary>
		/// �R���X�^���g�o�b�t�@�\����
		/// </summary>
		/// <param name="device">�f�o�C�X</param>
		/// <param name="bufferSize">�o�b�t�@�[�T�C�Y
		/// <returns></returns>
		HRESULT CreateConstantBuffer(ID3D12Device& device, const UINT& bufferSize);

		/// <summary>
		/// �}�b�v����
		/// </summary>
		/// <param name="data">�f�o�C�X</param>
		/// <param name="bufferSize">�o�b�t�@�[�T�C�Y</param>
		/// <returns></returns>
		HRESULT MapConstantBuffer(void* data, const UINT& bufferSize);

	public:
		/// <summary>
		/// �R���X�^���g�o�b�t�@�\����
		/// </summary>
		/// <param name="device">�f�o�C�X</param>
		/// <param name="data">�o�b�t�@�[�ɓ����f�[�^</param>
		/// <param name="bufferSize">�o�b�t�@�[�T�C�Y</param>
		/// <returns>Utility::RESULT::SUCCESS: ���� Utility::RESULT::FAILED: ���s</returns>
		Utility::RESULT Create(ID3D12Device& device, void* data, const UINT& bufferSize);

		/// <summary>
		/// �o�b�t�@�[��GPU��̃o�[�`�����A�h���X�擾
		/// </summary>
		/// <returns>�o�b�t�@�[��GPU��̃o�[�`�����A�h���X</returns>
		D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() {
			return m_constantBuffer->GetGPUVirtualAddress();
		}

		/// <summary>
		/// �o�b�t�@�[�擾
		/// </summary>
		/// <returns>�o�b�t�@�\</returns>
		ID3D12Resource& GetBuffer() {
			return *m_constantBuffer.Get();
		}

		/// <summary>
		/// �o�b�t�@�[�T�C�Y�擾
		/// </summary>
		/// <returns>�o�b�t�@�[�T�C�Y</returns>
		UINT& GetBufferSize() {
			return m_bufferSize;
		}

		/// <summary>
		/// �f�[�^�X�V �o�b�t�@�[�������̎w��T�C�Y���̃f�[�^���R�s�[����
		/// </summary>
		/// <param name="data">�f�[�^</param>
		void UpdateData(void* data) {
			std::memcpy((void*)m_mappedData, data, static_cast<size_t>(m_bufferSize));
		}
	};
}