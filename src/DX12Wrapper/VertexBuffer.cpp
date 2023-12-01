#include "VertexBuffer.h"

using namespace Utility;

namespace DX12Wrapper
{
	HRESULT VertexBuffer::CreateVertexBufferAndView(ID3D12Device& device, const UINT& sizeInBytes, const UINT& stribeInBytes)
	{
		// バッファー生成
		CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeInBytes);

		HRESULT result = device.CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(m_vertexBuffer.ReleaseAndGetAddressOf()));

		if (FAILED(result)) { return result; }

		// ビュー生成
		m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();	// バッファのGPU側の仮想アドレス
		m_vertexBufferView.SizeInBytes = sizeInBytes;	                                // 頂点の全サイズ
		m_vertexBufferView.StrideInBytes = stribeInBytes;	                    // 1頂点当たりのサイズ

		return result;
	}

	HRESULT VertexBuffer::MapVertexBuffer(void* vertexData, const UINT& sizeInBytes)
	{
		BYTE* mappedData;
		HRESULT result = m_vertexBuffer->Map(0, nullptr, (void**)&mappedData);
		if (FAILED(result)) { return result; }

		std::memcpy((void*)mappedData, vertexData, static_cast<size_t>(sizeInBytes));

		m_vertexBuffer->Unmap(0, nullptr);

		return result;
	}

	RESULT VertexBuffer::Create(
		ID3D12Device& device, void* vertexData, const UINT& sizeInBytes, const UINT& stribeInBytes)
	{
		// 頂点数記録
		m_vertexNum = sizeInBytes / stribeInBytes;

		// バッファーとビュー生成
		if (FAILED(CreateVertexBufferAndView(device, sizeInBytes, stribeInBytes))) { return RESULT::FAILED; }
		// マップ処理
		if (FAILED(MapVertexBuffer(vertexData, sizeInBytes))) { return RESULT::FAILED; }

		return RESULT::SUCCESS;
	}
}
