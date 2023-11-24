#include "Texture.h"
#include "Dx12GraphicsEngine.h"
#include "RenderTargetBuffer.h"
#include "DepthStencilBuffer.h"

using namespace DirectX;
using namespace Utility;

namespace DX12Wrapper
{
	Texture& Texture::operator=(const Texture& inst)
	{
		// �e�N�X�`���o�b�t�@�[�ƌ��т���
		this->m_textureBuffer = inst.m_textureBuffer.Get();

		// �V�F�[�_�[���\�[�X�Ƃ��ēo�^����ۂɕK�v�ȏ����Z�b�g
		DirectX::Image* img = new DirectX::Image();
		img->format = inst.m_image->format;
		this->m_image = img;

		this->m_metaData.mipLevels = 1;

		return *this;
	}

	HRESULT Texture::LoadTextureFromWICFile(const std::wstring& texturePath)
	{
		// �t�@�C���ǂݍ���
		HRESULT result = DirectX::LoadFromWICFile(
			texturePath.c_str(), DirectX::WIC_FLAGS_NONE, &m_metaData, m_scratchImage);
		if (FAILED(result)) { return result; }

		// �e�N�X�`���̐��f�[�^�擾
		m_image = m_scratchImage.GetImage(0, 0, 0);

		return result;
	}

	HRESULT Texture::LoadTextureFromDDSFile(const std::wstring& texturePath)
	{
		// �t�@�C���ǂݍ��݁@
		HRESULT result = DirectX::LoadFromDDSFile(
			texturePath.c_str(), DirectX::DDS_FLAGS_NONE, &m_metaData, m_scratchImage);
		if (FAILED(result)) { return result; }

		// �e�N�X�`���̐��f�[�^�擾
		m_image = m_scratchImage.GetImage(0, 0, 0);

		return result;
	}

	HRESULT Texture::CreateUploadAndTextureBuffer(ID3D12Device& device)
	{
		// �A�b�v���[�h�p�̒��ԃo�b�t�@�[����
		CD3DX12_HEAP_PROPERTIES uploadHeapProp(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC uploadResDesc = CD3DX12_RESOURCE_DESC::Buffer(
			Utility::AlignmentedSize(m_image->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT) * m_image->height);

		HRESULT result = device.CreateCommittedResource(
			&uploadHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&uploadResDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(m_uploadBuffer.ReleaseAndGetAddressOf()));
		if (FAILED(result)) { return result; }

		// �A�b�v���[�h��̃o�b�t�@�[����
		CD3DX12_HEAP_PROPERTIES texHeapProp(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_RESOURCE_DESC texResDesc;
		texResDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(m_metaData.dimension);
		texResDesc.Alignment = 0;
		texResDesc.Width = m_metaData.width;
		texResDesc.Height = m_metaData.height;
		texResDesc.DepthOrArraySize = m_metaData.arraySize;
		texResDesc.MipLevels = m_metaData.mipLevels;
		texResDesc.Format = m_metaData.format;
		texResDesc.SampleDesc.Count = 1;
		texResDesc.SampleDesc.Quality = 0;
		texResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		texResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		result = device.CreateCommittedResource(
			&texHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&texResDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(m_textureBuffer.ReleaseAndGetAddressOf()));
		if (FAILED(result)) { return result; }

		return result;
	}

	HRESULT Texture::CreateUploadAndCubeTextureBuffer(ID3D12Device& device)
	{
		// �A�b�v���[�h��̃o�b�t�@�[����
		CD3DX12_HEAP_PROPERTIES texHeapProp(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_RESOURCE_DESC texResDesc;
		texResDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(m_metaData.dimension);
		texResDesc.Alignment = 0;
		texResDesc.Width = m_metaData.width;
		texResDesc.Height = m_metaData.height;
		texResDesc.DepthOrArraySize = m_metaData.arraySize;
		texResDesc.MipLevels = m_metaData.mipLevels;
		texResDesc.Format = m_metaData.format;
		texResDesc.SampleDesc.Count = 1;
		texResDesc.SampleDesc.Quality = 0;
		texResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		texResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		HRESULT result = device.CreateCommittedResource(
			&texHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&texResDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(m_textureBuffer.ReleaseAndGetAddressOf()));
		if (FAILED(result)) { return result; }

		m_subresources.resize(m_scratchImage.GetImageCount());
		for (size_t idx = 0; idx < m_scratchImage.GetImageCount(); idx++) {
			const DirectX::Image img = m_scratchImage.GetImages()[idx];
			m_subresources[idx].pData = img.pixels;
			m_subresources[idx].RowPitch = img.rowPitch;
			m_subresources[idx].SlicePitch = img.slicePitch;
		}

		auto totalBytes = GetRequiredIntermediateSize(m_textureBuffer.Get(), 0, UINT(m_subresources.size()));

		// �A�b�v���[�h�p�̒��ԃo�b�t�@�[����
		CD3DX12_RESOURCE_DESC uploadResDesc = CD3DX12_RESOURCE_DESC::Buffer(totalBytes);
		CD3DX12_HEAP_PROPERTIES uploadHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		result = device.CreateCommittedResource(
			&uploadHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&uploadResDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(m_uploadBuffer.ReleaseAndGetAddressOf()));
		if (FAILED(result)) { return result; }


		return result;
	}

	HRESULT Texture::MapTexture()
	{
		// �}�b�v
		uint8_t* texMap = nullptr;
		HRESULT result = m_uploadBuffer->Map(0, nullptr, (void**)&texMap);
		if (FAILED(result)) { return result; }

		// �f�[�^�R�s�[
		uint8_t* srcAddress = m_image->pixels;
		size_t alignRowPitch = Utility::AlignmentedSize(m_image->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);

		for (size_t y = 0; y < m_image->height; y++) {
			std::copy_n(srcAddress, alignRowPitch, texMap);
			srcAddress += m_image->rowPitch;
			texMap += alignRowPitch;
		}
		m_uploadBuffer->Unmap(0, nullptr);

		return result;
	}

	HRESULT Texture::CopyTexture(ID3D12Device& device)
	{
		RenderingContext& renderContext = Dx12GraphicsEngine::GetRenderingContext();
		ID3D12CommandAllocator& cmdAllocator = Dx12GraphicsEngine::CmdAllocator();
		ID3D12CommandQueue& cmdQueue = Dx12GraphicsEngine::CmdQueue();

		// �R�s�[��
		D3D12_TEXTURE_COPY_LOCATION src = {};
		src.pResource = m_uploadBuffer.Get();
		src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		src.PlacedFootprint.Offset = 0;
		src.PlacedFootprint.Footprint.Width = m_metaData.width;
		src.PlacedFootprint.Footprint.Height = m_metaData.height;
		src.PlacedFootprint.Footprint.Depth = m_metaData.depth;
		src.PlacedFootprint.Footprint.RowPitch =
			Utility::AlignmentedSize(m_image->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);
		src.PlacedFootprint.Footprint.Format = m_image->format;

		// �R�s�[��
		D3D12_TEXTURE_COPY_LOCATION dst = {};
		dst.pResource = m_textureBuffer.Get();
		dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		dst.SubresourceIndex = 0;

		// �t�F���X����
		Microsoft::WRL::ComPtr<ID3D12Fence> fence = nullptr;
		UINT fenceVal = 0;
		HRESULT result = device.CreateFence(
			fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.ReleaseAndGetAddressOf()));
		if (FAILED(result)) { return result; }

		// ���ԃo�b�t�@�[�̓��e���R�s�[��o�b�t�@�[�փR�s�[
		{
			renderContext.CopyTextureRegion(src, dst);
			renderContext.TransitionResourceState(
				*m_textureBuffer.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_PRESENT);

			renderContext.Close();

			// �R�}���h���X�g���s
			ID3D12CommandList* cmdLists[] = { &Dx12GraphicsEngine::CmdList() };
			cmdQueue.ExecuteCommandLists(_countof(cmdLists), cmdLists);
			cmdQueue.Signal(fence.Get(), ++fenceVal);
			if (fence->GetCompletedValue() != fenceVal) {
				// �C�x���g�n���h���擾
				auto event = CreateEvent(nullptr, false, false, nullptr);

				fence->SetEventOnCompletion(fenceVal, event);

				// �C�x���g����������܂ő҂�������
				WaitForSingleObject(event, INFINITE);

				// �C�x���g�n���h�������
				CloseHandle(event);
			}

			cmdAllocator.Reset();
			renderContext.Reset(cmdAllocator);
		}

		return result;
	}

	HRESULT Texture::CopyCubeTexture()
	{
		RenderingContext& renderContext = Dx12GraphicsEngine::GetRenderingContext();
		ID3D12GraphicsCommandList& commandList = Dx12GraphicsEngine::CmdList();
		ID3D12CommandAllocator& cmdAllocator = Dx12GraphicsEngine::CmdAllocator();
		ID3D12CommandQueue& cmdQueue = Dx12GraphicsEngine::CmdQueue();

		UpdateSubresources(
			&commandList, m_textureBuffer.Get(), m_uploadBuffer.Get(),
			0, 0, UINT(m_subresources.size()), m_subresources.data());
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			m_textureBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PRESENT);
		commandList.ResourceBarrier(1, &barrier);

		// �t�F���X����
		Microsoft::WRL::ComPtr<ID3D12Fence> fence = nullptr;
		UINT fenceVal = 0;
		HRESULT result = Dx12GraphicsEngine::Device().CreateFence(
			fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.ReleaseAndGetAddressOf()));
		if (FAILED(result)) { return result; }

		renderContext.Close();

		// �R�}���h���X�g���s
		ID3D12CommandList* cmdLists[] = { &commandList };
		cmdQueue.ExecuteCommandLists(_countof(cmdLists), cmdLists);
		cmdQueue.Signal(fence.Get(), ++fenceVal);
		if (fence->GetCompletedValue() != fenceVal) {
			// �C�x���g�n���h���擾
			auto event = CreateEvent(nullptr, false, false, nullptr);

			fence->SetEventOnCompletion(fenceVal, event);

			// �C�x���g����������܂ő҂�������
			WaitForSingleObject(event, INFINITE);

			// �C�x���g�n���h�������
			CloseHandle(event);
		}

		cmdAllocator.Reset();
		renderContext.Reset(cmdAllocator);

		return result;
	}

	void Texture::SetTextureData(
		uint8_t* data, const size_t& stride, const size_t& dataNum, const size_t& width, const size_t& height, const DXGI_FORMAT& format)
	{
		Utility::SafetyDelete<const DirectX::Image>(m_image);

		DirectX::Image* img = new DirectX::Image();
		img->width = width;
		img->height = height;
		img->format = format;
		img->rowPitch = stride * width;
		img->slicePitch = img->rowPitch * height;
		img->pixels = data;
		m_image = img;

		m_metaData.width = width;
		m_metaData.height = height;
		m_metaData.depth = 1;
		m_metaData.arraySize = 1;
		m_metaData.mipLevels = 1;
		m_metaData.format = format;
		m_metaData.dimension = DirectX::TEX_DIMENSION_TEXTURE2D;
	}

	RESULT Texture::CreateTextureFromWIC(const std::wstring& texturePath)
	{
		ID3D12Device& device = Dx12GraphicsEngine::Device();

		// �t�@�C���ǂݍ���
		if (FAILED(LoadTextureFromWICFile(texturePath))) { return RESULT::FAILED; }
		// �o�b�t�@�[����
		if (FAILED(CreateUploadAndTextureBuffer(device))) { return RESULT::FAILED; }
		// �}�b�v����
		if (FAILED(MapTexture())) { return RESULT::FAILED; }
		// �A�b�v���[�h�o�b�t�@�[�̓��e���e�N�X�`���o�b�t�@�[�փR�s�[
		if (FAILED(CopyTexture(device))) { return RESULT::FAILED; }

		return RESULT::SUCCESS;
	}

	RESULT Texture::CreateTextureFromDDS(const std::wstring& texturePath)
	{
		ID3D12Device& device = Dx12GraphicsEngine::Device();

		// �t�@�C���ǂݍ���
		if (FAILED(LoadTextureFromDDSFile(texturePath))) { return RESULT::FAILED; }
		// �o�b�t�@�[����
		if (FAILED(CreateUploadAndTextureBuffer(device))) { return RESULT::FAILED; }
		// �}�b�v����
		if (FAILED(MapTexture())) { return RESULT::FAILED; }
		// �A�b�v���[�h�o�b�t�@�[�̓��e���e�N�X�`���o�b�t�@�[�փR�s�[
		if (FAILED(CopyTexture(device))) { return RESULT::FAILED; }

		return RESULT::SUCCESS;
	}

	RESULT Texture::CreateTextureFromConstantData(
		uint8_t* data, const size_t& stride, const size_t& dataNum,
		const size_t& width, const size_t& height, const DXGI_FORMAT& format)
	{
		ID3D12Device& device = Dx12GraphicsEngine::Device();

		// �e�N�X�`�������p�f�[�^�p��
		SetTextureData(data, stride, dataNum, width, height, format);
		// �o�b�t�@�[����
		if (FAILED(CreateUploadAndTextureBuffer(device))) { return RESULT::FAILED; }
		// �}�b�v����
		if (FAILED(MapTexture())) { return RESULT::FAILED; }
		// �A�b�v���[�h�o�b�t�@�[�̓��e���e�N�X�`���o�b�t�@�[�փR�s�[
		if (FAILED(CopyTexture(device))) { return RESULT::FAILED; }
		return RESULT::SUCCESS;
	}

	void Texture::CreateTextureFromRenderTarget(RenderTargetBuffer& renderTargetBuffer)
	{
		// �����_�[�^�[�Q�b�g�o�b�t�@�[�ƃe�N�X�`���o�b�t�@�[�����т���
		m_textureBuffer = &renderTargetBuffer.GetBuffer();

		// �V�F�[�_�[���\�[�X�Ƃ��ēo�^����ۂɕK�v�ȏ����Z�b�g
		DirectX::Image* img = new DirectX::Image();
		img->format = renderTargetBuffer.GetBuffer().GetDesc().Format;
		m_image = img;

		m_metaData.mipLevels = 1;
	}

	void Texture::CreateTextureFromDepthStencil(DepthStencilBuffer& depthStencilBuffer)
	{
		// �f�v�X�X�e���V���o�b�t�@�[�ƃe�N�X�`���o�b�t�@�[�����т���
		m_textureBuffer = &depthStencilBuffer.GetBuffer();

		// �V�F�[�_�[���\�[�X�Ƃ��ēo�^����ۂɕK�v�ȏ����Z�b�g
		DirectX::Image* img = new DirectX::Image();
		img->format = DXGI_FORMAT_R32_FLOAT;
		m_image = img;

		m_metaData.mipLevels = 1;
	}

	RESULT Texture::CreateCubeTextureFromDDS(Dx12GraphicsEngine& graphicsEngine, const std::wstring& texturePath)
	{
		ID3D12Device& device = Dx12GraphicsEngine::Device();

		// �t�@�C���ǂݍ���
		if (FAILED(LoadTextureFromDDSFile(texturePath))) { return RESULT::FAILED; }
		// �o�b�t�@�[����
		if (FAILED(CreateUploadAndCubeTextureBuffer(device))) { return RESULT::FAILED; }
		// �A�b�v���[�h�o�b�t�@�[�̓��e���e�N�X�`���o�b�t�@�[�փR�s�[
		if (FAILED(CopyCubeTexture())) { return RESULT::FAILED; }

		return RESULT::SUCCESS;
	}

}