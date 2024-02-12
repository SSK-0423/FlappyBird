#pragma once

#include <d3d12.h>
#include <DirectXTex.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"DirectXTex.lib")
#pragma comment(lib,"dxguid.lib")

#include "Dx12GraphicsEngine.h"

#include "../Utility/EngineUtility.h"

namespace DX12Wrapper
{
	class RenderTargetBuffer;
	class DepthStencilBuffer;

	/// <summary>
	/// �V�F�[�_�[���\�[�X���e�N�X�`���Ƃ��Ĉ������߂̃N���X
	/// </summary>
	class Texture {
	public:
		Texture() = default;
		~Texture() = default;

		Texture(const Texture& inst);
		Texture& operator=(const Texture& inst);

		// std::unordered_map�̃L�[�Ƃ��Ďg�p���邽��
		bool operator==(const Texture& inst) const;

		friend std::hash<Texture>;

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> m_uploadBuffer = nullptr;		// ���ԃo�b�t�@�[(�A�b�v���[�h��)
		Microsoft::WRL::ComPtr<ID3D12Resource> m_textureBuffer = nullptr;	// �e�N�X�`���o�b�t�@�[(�A�b�v���[�h��)

		std::vector<D3D12_SUBRESOURCE_DATA> m_subresources; // �L���[�u�e�N�X�`���p�T�u���\�[�X

		// ScratchImage::GetImage�̖߂�l��const�����Ă��邽��
		const DirectX::Image* m_image = nullptr;	// �e�N�X�`���̐��f�[�^
		DirectX::TexMetadata m_metaData;	        // �e�N�X�`���̃��^���
		DirectX::ScratchImage m_scratchImage;    // 

		/// <summary>
		/// WIC�Ή��̃t�@�C��(bmp,png,jpeg,tiff�Ȃ�)��ǂݍ���
		/// </summary>
		/// <param name="texPath">�e�N�X�`���ւ̃p�X</param>
		/// <returns></returns>
		HRESULT LoadTextureFromWICFile(const std::wstring& texturePath);

		/// <summary>
		/// DDS�t�@�C���ǂݍ���
		/// </summary>
		/// <param name="texturePath">�e�N�X�`���ւ̃p�X</param>
		/// <returns></returns>
		HRESULT LoadTextureFromDDSFile(const std::wstring& texturePath);

		/// <summary>
		/// ���ԃo�b�t�@�[�ƃe�N�X�`���o�b�t�@�[����
		/// </summary>
		/// <param name="device">�f�o�C�X</param>
		/// <returns></returns>
		HRESULT CreateUploadAndTextureBuffer(ID3D12Device& device);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="device"></param>
		/// <returns></returns>
		HRESULT CreateUploadAndCubeTextureBuffer(ID3D12Device& device);

		/// <summary>
		/// �}�b�v����
		/// </summary>
		/// <returns></returns>
		HRESULT MapTexture();

		/// <summary>
		/// �A�b�v���[�h�o�b�t�@�[�̓��e���e�N�X�`���o�b�t�@�[�փR�s�[
		/// </summary>
		/// <param name="device">�f�o�C�X</param>
		/// <param name="graphicsEngine">�O���t�B�N�X�G���W��</param>
		/// <returns></returns>
		HRESULT CopyTexture(ID3D12Device& device);

		/// <summary>
		/// �A�b�v���[�h�o�b�t�@�[�̓��e���L���[�u�e�N�X�`���o�b�t�@�[�փR�s�[
		/// </summary>
		/// <returns></returns>
		HRESULT CopyCubeTexture();

		/// <summary>
		/// RGBA���̃f�\�^���摜�ł͂Ȃ����ڃZ�b�g����
		/// </summary>
		/// <param name="data"></param>
		/// <param name="stride"></param>
		/// <param name="dataNum"></param>
		/// <param name="width"></param>
		/// <param name="height"></param>
		/// <param name="format"></param>
		void SetTextureData(
			uint8_t* data, const size_t& stride, const size_t& dataNum, const size_t& width, const size_t& height, const DXGI_FORMAT& format);

	public:
		/// <summary>
		/// WIC�Ή��̃t�@�C��((bmp,png,jpeg,tiff�Ȃ�)����e�N�X�`���𐶐�
		/// </summary>
		/// <param name="graphicsEngine">�O���t�B�N�X�G���W��</param>
		/// <param name="texturePath">�e�N�X�`���ւ̃p�X</param>
		/// <returns>�����FUtility::RESULT::SUCCESS ���s�FUtility::RESULT::FAILED</returns>
		Utility::RESULT CreateTextureFromWIC(const std::wstring& texturePath);

		/// <summary>
		/// DDS�t�@�C������e�N�X�`���𐶐�
		/// </summary>
		/// <param name="graphicsEngine">�O���t�B�N�X�G���W��</param>
		/// <param name="texturePath">�e�N�X�`���ւ̃p�X</param>
		/// <returns>�����FUtility::RESULT::SUCCESS ���s�FUtility::RESULT::FAILED</returns>
		Utility::RESULT CreateTextureFromDDS(const std::wstring& texturePath);

		/// <summary>
		/// �p�ӂ���RGBA�f�[�^����e�N�X�`������
		/// </summary>
		/// <param name="graphicsEngine"></param>
		/// <param name="data"></param>
		/// <param name="width"></param>
		/// <param name="height"></param>
		/// <returns></returns>
		Utility::RESULT CreateTextureFromConstantData(
			uint8_t* data, const size_t& stride, const size_t& dataNum,
			const size_t& width, const size_t& height, const DXGI_FORMAT& format);

		/// <summary>
		/// �����_�[�^�[�Q�b�g����e�N�X�`������
		/// </summary>
		/// <param name="renderTargetBuffer">�����_�[�^�[�Q�b�g�o�b�t�@�[</param>
		void CreateTextureFromRenderTarget(RenderTargetBuffer& renderTargetBuffer);

		/// <summary>
		/// �f�v�X�X�e���V���o�b�t�@�[����e�N�X�`������
		/// </summary>
		/// <param name="depthStencilBuffer">�f�v�X�X�e���V���o�b�t�@�[</param>
		void CreateTextureFromDepthStencil(DepthStencilBuffer& depthStencilBuffer);

		Utility::RESULT CreateCubeTextureFromDDS(Dx12GraphicsEngine& graphicsEngine, const std::wstring& texturePath);

		/// <summary>
		/// �e�N�X�`���o�b�t�@�[�擾
		/// </summary>
		/// <returns>�e�N�X�`���o�b�t�@�[</returns>
		ID3D12Resource& GetBuffer() const {
			return *m_textureBuffer.Get();
		}

		/// <summary>
		/// �e�N�X�`���̃��^�f�[�^�擾
		/// </summary>
		/// <returns>�e�N�X�`���̃��^�f�[�^</returns>
		DirectX::TexMetadata& GetTexMetadata() {
			return m_metaData;
		}

		/// <summary>
		/// �e�N�X�`���̐��f�[�^�擾
		/// </summary>
		/// <returns>�e�N�X�`���̐��f�[�^</returns>
		const DirectX::Image& GetImage() {
			return *m_image;
		}
	};
}

// std::unordered_map�̃L�[�Ƃ��Ďg�p���邽�߂Ƀn�b�V���֐����`
namespace std
{
	template<>
	class hash<DX12Wrapper::Texture>
	{
	public:
		std::size_t operator()(const DX12Wrapper::Texture& inst) const
		{
			return std::hash<size_t>()((size_t)inst.m_textureBuffer.Get());
		}
	};
}