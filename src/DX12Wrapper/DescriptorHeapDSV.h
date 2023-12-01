#pragma once

#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "../Utility/EngineUtility.h"


namespace DX12Wrapper
{
	class DepthStencilBuffer;

	class DescriptorHeapDSV
	{
	public:
		DescriptorHeapDSV() = default;
		~DescriptorHeapDSV() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_dsvHeap = nullptr;		// ディスクリプタヒープ
		SIZE_T m_handleIncrimentSize = 0;										// ハンドルのインクリメントサイズ
		UINT m_registedDSVNum = 0;									            // ヒープに登録されたディスクリプタ数

		/// <summary>
		/// ディスクリプタヒープ生成
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <returns></returns>
		HRESULT CreateDescriptorHeap(ID3D12Device& device);

	public:
		/// <summary>
		/// DSV用ディスクリプタヒープ生成 これを実行することでディスクリプタヒープが扱えるようになる
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <returns>RESULT::SUCCESS: 成功 RESULT::FAILED: 失敗</returns>
		Utility::RESULT Create(ID3D12Device& device);

		/// <summary>
		/// ディスクリプタヒープハンドルのインクリメントサイズ取得
		/// </summary>
		/// <returns></returns>
		SIZE_T GetHandleIncrimentSize() {
			return m_handleIncrimentSize;
		}

		/// <summary>
		/// 登録済みのディスクリプタ数取得
		/// </summary>
		/// <returns></returns>
		UINT GetRegistedDescriptorNum() {
			return m_registedDSVNum;
		}

		/// <summary>
		/// ディスクリプタ登録
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <param name="buffer">デプスステンシルバッファー</param>
		void RegistDescriptor(ID3D12Device& device, DepthStencilBuffer& buffer);

		/// <summary>
		/// CPUのディスクリプタヒープの先頭ハンドルを取得
		/// </summary>
		/// <returns></returns>
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForHeapStart() {
			return m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
		}
	};
}