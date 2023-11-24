#include "IndexBuffer.h"

using namespace Utility;

namespace DX12Wrapper
{
	HRESULT IndexBuffer::CreateIndexBufferAndView(ID3D12Device& device, const std::vector<UINT>& index)
	{
		CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_UPLOAD);
		UINT bufferSize = static_cast<UINT>(sizeof(index[0]) * index.size());
		CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

		HRESULT result = device.CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(m_indexBuffer.ReleaseAndGetAddressOf()));

		if (FAILED(result)) { return result; }

		// ビュー生成
		m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
		m_indexBufferView.SizeInBytes = bufferSize;
		m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;

		return result;
	}

	HRESULT IndexBuffer::MapIndexBuffer(const std::vector<UINT>& index)
	{
		HRESULT result = m_indexBuffer->Map(0, nullptr, (void**)&m_indexMap);
		if (FAILED(result)) { return result; }

		std::copy(std::begin(index), std::end(index), m_indexMap);

		m_indexBuffer->Unmap(0, nullptr);

		return result;
	}

	RESULT IndexBuffer::Create(ID3D12Device& device, const std::vector<UINT>& index)
	{
		// インデックス数記録
		m_indexNum = static_cast<UINT>(index.size());

		// インデックスバッファーとビュー生成
		if (FAILED(CreateIndexBufferAndView(device, index))) { return RESULT::FAILED; }
		// マップ処理
		if (FAILED(MapIndexBuffer(index))) { return RESULT::FAILED; }

		return RESULT::SUCCESS;
	}

}