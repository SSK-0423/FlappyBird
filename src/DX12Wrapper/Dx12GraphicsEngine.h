#pragma once
#include "../Utility/EngineUtility.h"
#include "RenderingContext.h"
#include "RenderTargetBuffer.h"
#include "DescriptorHeapRTV.h"
#include "DepthStencilBuffer.h"
#include "DescriptorHeapDSV.h"
#include "DescriptorHeapCBV_SRV_UAV.h"

#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dx12.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include <GraphicsMemory.h>

#pragma comment(lib,"DirectXTK12.lib")

#include <wrl.h>

namespace DX12Wrapper
{
	class Dx12GraphicsEngine
	{

	public:
		/// <summary>
		/// DirectXで描画を行うための初期化処理
		/// </summary>
		/// <param name="hwnd">ウィンドウハンドル</param>
		/// <param name="windowWidth">ウィンドウ幅</param>
		/// <param name="windowHeight">ウィンドウ高</param>
		/// <param name="dxgiFactory">ファクトリー</param>
		/// <returns>Utility::RESULT::SUCCESS: 成功 Utility::RESULT::FAILED: 失敗</returns>
		static Utility::RESULT Init(const HWND& hwnd,
			const UINT& windowWidth, const UINT& windowHeight,
			const UINT& viewportWidth, const UINT& viewportHeight,
			const UINT& scissorRectWidth, const UINT& scissorRectHeight);

		/// <summary>
		/// デバイス取得
		/// </summary>
		/// <returns></returns>
		static ID3D12Device& Device();
		/// <summary>
		/// コマンドリスト取得
		/// </summary>
		/// <returns></returns>
		static ID3D12GraphicsCommandList& CmdList();
		/// <summary>
		/// コマンドアロケータ取得
		/// </summary>
		/// <returns></returns>
		static ID3D12CommandAllocator& CmdAllocator();
		/// <summary>
		/// コマンドキュー取得
		/// </summary>
		/// <returns></returns>
		static ID3D12CommandQueue& CmdQueue();
		/// <summary>
		/// スワップチェーン取得
		/// </summary>
		/// <returns></returns>
		static IDXGISwapChain3& SwapChain();

		/// <summary>
		/// 1フレームの描画開始
		/// </summary>
		static void BeginDraw();
		/// <summary>
		/// 1フレームの描画終了
		/// </summary>
		static void EndDraw();
		/// <summary>
		/// フレームレンダーターゲットセット
		/// </summary>
		static void SetFrameRenderTarget(const CD3DX12_VIEWPORT& viewport, const CD3DX12_RECT& scissorRect);

		/// <summary>
		/// レンダリングコンテキスト取得
		/// </summary>
		/// <returns></returns>
		static DX12Wrapper::RenderingContext& GetRenderingContext();

		static DX12Wrapper::DescriptorHeapRTV& GetFrameBufferDescriptorHeap();

		static const SIZE GetFrameRenderTargetSize();
		static const CD3DX12_VIEWPORT& GetViewport();
		static const CD3DX12_RECT& GetScissorRect();

	private:
		Dx12GraphicsEngine() = default;
		~Dx12GraphicsEngine();
		Dx12GraphicsEngine(const Dx12GraphicsEngine& inst) = delete;
		void operator=(const Dx12GraphicsEngine& inst) = delete;

		static constexpr UINT DOUBLE_BUFFER = 2;	// ダブルバッファリング

		// DXGI関連
		static Microsoft::WRL::ComPtr<IDXGIFactory4> m_dxgiFactory;
		static Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapchain;

		// DirectX12初期化関連
		static Microsoft::WRL::ComPtr<ID3D12Device> m_device;
		static Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_cmdAllocator;
		static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_cmdList;
		static Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_cmdQueue;

		// フェンス関連
		static Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
		static UINT m_fenceVal;

		// ウィンドウ関連
		static HWND m_hwnd;
		static UINT m_windowWidth;
		static UINT m_windowHeight;

		/// <summary>
		/// デバッグレイヤーを有効にする
		/// </summary>
		/// <returns></returns>
		static HRESULT EnableDebugLayer();
		/// <summary>
		/// デバイスとファクトリー生成
		/// </summary>
		/// <returns></returns>
		static HRESULT CreateDeviceAndDXGIFactory();
		/// <summary>
		/// コマンドアロケータ、リスト、キュー生成
		/// </summary>
		/// <returns></returns>
		static HRESULT CreateCommandX();
		/// <summary>
		/// スワップチェーン生成
		/// </summary>
		/// <param name="hwnd">ウィンドウハンドル</param>
		/// <param name="windowWidth">ウィンドウ幅</param>
		/// <param name="windowHeightconst">ウィンドウ高</param>
		/// <param name="dxgiFactory">ファクトリー</param>
		/// <returns></returns>
		static HRESULT CreateSwapChain(
			const HWND& hwnd, const UINT& windowWidth, const UINT& windowHeight,
			const Microsoft::WRL::ComPtr<IDXGIFactory4>& dxgiFactory);
		/// <summary>
		/// フェンス生成
		/// </summary>
		/// <returns></returns>
		static HRESULT CreateFence();

		static DX12Wrapper::RenderingContext m_renderContext;	            // レンダリングコンテキスト
		static DX12Wrapper::RenderTargetBuffer m_frameBuffers[2];	        // フレームバッファ
		static DX12Wrapper::DescriptorHeapRTV m_frameHeap;	                // フレームバッファ用ディスクリプタヒープ	

		static DX12Wrapper::DepthStencilBufferData m_depthStencilBufferData;	// デプスステンシルバッファーの設定
		static DX12Wrapper::DepthStencilBuffer m_depthStencilBuffer;		    // デプスステンシルバッファー
		static DX12Wrapper::DescriptorHeapDSV m_dsvHeap;					    // デプスステンシル用ヒープ

		static DX12Wrapper::DescriptorHeapCBV_SRV_UAV m_imguiHeap;

		// フォントレンダリング関連
		static DirectX::GraphicsMemory* m_graphicsMemory;

		static CD3DX12_VIEWPORT m_viewport;
		static CD3DX12_RECT m_scissorRect;

		/// <summary>
		/// フレームバッファ用のレンダーターゲット生成
		/// </summary>
		/// <returns></returns>
		static Utility::RESULT CreateFrameRenderTarget();

		static Utility::RESULT CreateImguiDescriptorHeap();

		static DescriptorHeapCBV_SRV_UAV& GetImguiDescriptorHeap();
	};
}