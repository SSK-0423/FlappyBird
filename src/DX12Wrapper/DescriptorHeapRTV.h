#pragma once
#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "../Utility/EngineUtility.h"


namespace DX12Wrapper
{
	class RenderTargetBuffer;

	/// <summary>
	/// レンダーターゲットビュー用のディスクリプタヒープ
	/// </summary>
	class DescriptorHeapRTV
	{
	public:
		DescriptorHeapRTV() = default;
		~DescriptorHeapRTV() = default;

		static constexpr UINT MAXDESCRIPTORNUM = 8;							    // 登録可能なディスクリプタ数

	private:
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap = nullptr;	    // ディスクリプタヒープ
		SIZE_T m_handleIncrimentSize = 0;										// ハンドルのインクリメントサイズ
		UINT m_registedRTVNum = 0;									            // ヒープに登録されたディスクリプタ数
		UINT m_nextHandleLocation = 1;

		/// <summary>
		/// ディスクリプタヒープ生成
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <returns></returns>
		HRESULT CreateDescriptorHeap(ID3D12Device& device);

	public:
		/// <summary>
		/// RTV用ディスクリプタヒープ生成 これを実行することでディスクリプタヒープが扱えるようになる
		/// </summary>
		/// <param name="device"></param>
		/// <returns>Utility::RESULT::SUCCESS: 成功 Utility::RESULT::FAILED: 失敗</returns>
		Utility::RESULT Create(ID3D12Device& device);

		/// <summary>
		/// CPUのディスクリプタヒープの先頭ハンドルを取得
		/// この関数を実行するとGetNextCPUDescriptorHandleで取得できるハンドルが2番目に初期化される
		/// </summary>
		/// <returns></returns>
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForHeapStart() {
			m_nextHandleLocation = 1;
			return m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
		}

		/// <summary>
		/// 次のディスクリプタのハンドル取得
		/// 初期は2番目のハンドルが取得できる
		/// 登録された数を超えたハンドルを取得しようとすると登録された中の最後のハンドルを返す
		/// </summary>
		/// <returns></returns>
		D3D12_CPU_DESCRIPTOR_HANDLE GetNextCPUDescriptorHandle();

		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStart() {
			m_nextHandleLocation = 1;
			return m_rtvHeap->GetGPUDescriptorHandleForHeapStart();
		}

		/// <summary>
		/// ディスクリプタヒープハンドルのインクリメントサイズ取得
		/// </summary>
		/// <returns></returns>
		SIZE_T GetHandleIncrimentSize() {
			return m_handleIncrimentSize;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		UINT GetRegistedDescriptorNum() {
			return m_registedRTVNum;
		}

		/// <summary>
		/// ディスクリプタ登録
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <param name="buffer">レンダーターゲットバッファー</param>
		/// <param name="format">レンダーターゲットのフォーマット デフォルト値: DXGI_FORMAT_R8G8B8A8_UNORM</param>
		void RegistDescriptor(
			ID3D12Device& device, RenderTargetBuffer& buffer, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM,
			bool isCubeMap = false);
	};
}