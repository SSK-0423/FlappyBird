#pragma once
#include <d3d12.h>

#pragma comment(lib,"d3d12.lib")

#include "../Utility/EngineUtility.h"

namespace DX12Wrapper
{
	/// <summary>
	/// コンスタントバッファ―クラス
	/// </summary>
	class ConstantBuffer {
	public:
		ConstantBuffer() = default;
		~ConstantBuffer() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> m_constantBuffer = nullptr;	// バッファー
		BYTE* m_mappedData = nullptr;	                                    // データのマップ先
		UINT m_bufferSize = 0;	                                            // バッファーサイズ

		/// <summary>
		/// コンスタントバッファ―生成
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <param name="bufferSize">バッファーサイズ
		/// <returns></returns>
		HRESULT CreateConstantBuffer(ID3D12Device& device, const UINT& bufferSize);

		/// <summary>
		/// マップ処理
		/// </summary>
		/// <param name="data">デバイス</param>
		/// <param name="bufferSize">バッファーサイズ</param>
		/// <returns></returns>
		HRESULT MapConstantBuffer(void* data, const UINT& bufferSize);

	public:
		/// <summary>
		/// コンスタントバッファ―生成
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <param name="data">バッファーに入れるデータ</param>
		/// <param name="bufferSize">バッファーサイズ</param>
		/// <returns>Utility::RESULT::SUCCESS: 成功 Utility::RESULT::FAILED: 失敗</returns>
		Utility::RESULT Create(ID3D12Device& device, void* data, const UINT& bufferSize);

		/// <summary>
		/// バッファーのGPU上のバーチャルアドレス取得
		/// </summary>
		/// <returns>バッファーのGPU上のバーチャルアドレス</returns>
		D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() {
			return m_constantBuffer->GetGPUVirtualAddress();
		}

		/// <summary>
		/// バッファー取得
		/// </summary>
		/// <returns>バッファ―</returns>
		ID3D12Resource& GetBuffer() {
			return *m_constantBuffer.Get();
		}

		/// <summary>
		/// バッファーサイズ取得
		/// </summary>
		/// <returns>バッファーサイズ</returns>
		UINT& GetBufferSize() {
			return m_bufferSize;
		}

		/// <summary>
		/// データ更新 バッファー生成時の指定サイズ分のデータをコピーする
		/// </summary>
		/// <param name="data">データ</param>
		void UpdateData(void* data) {
			std::memcpy((void*)m_mappedData, data, static_cast<size_t>(m_bufferSize));
		}
	};
}