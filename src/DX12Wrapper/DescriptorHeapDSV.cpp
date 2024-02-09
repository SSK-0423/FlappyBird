#include "DescriptorHeapDSV.h"
#include "DepthStencilBuffer.h"

using namespace Utility;

namespace DX12Wrapper
{
	HRESULT DescriptorHeapDSV::CreateDescriptorHeap(ID3D12Device& device)
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		heapDesc.NumDescriptors = 1;
		heapDesc.NodeMask = 0;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		HRESULT result = device.CreateDescriptorHeap(
			&heapDesc, IID_PPV_ARGS(m_dsvHeap.ReleaseAndGetAddressOf()));
		if (FAILED(result)) { return result; }

		return result;
	}

	RESULT DescriptorHeapDSV::Create(ID3D12Device& device)
	{
		// ハンドルのインクリメントサイズ取得
		m_handleIncrimentSize =
			static_cast<SIZE_T>(device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV));

		// ディスクリプタヒープ生成
		if (FAILED(CreateDescriptorHeap(device))) { return RESULT::FAILED; }

		return RESULT::SUCCESS;
	}

	void DescriptorHeapDSV::RegistDescriptor(ID3D12Device& device, DepthStencilBuffer& buffer, int registerNo)
	{
		// ハンドル取得
		auto handle = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();

		if (registerNo == NEXT_REGISTER)
		{
			handle.ptr += m_registedDSVNum * m_handleIncrimentSize;
		}
		else
		{
			handle.ptr += registerNo * m_handleIncrimentSize;
		}

		// デプスステンシルビュー設定
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = buffer.GetDepthStencilBufferData().depthFormat;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

		//デプスステンシルビュー生成
		device.CreateDepthStencilView(&buffer.GetBuffer(), &dsvDesc, handle);

		// 次のレジスタを指定した場合は登録されたリソース数をインクリメント
		if (registerNo == NEXT_REGISTER)
		{
			// 登録済みのディスクリプタ数をインクリメント
			m_registedDSVNum++;
		}
	}
}
