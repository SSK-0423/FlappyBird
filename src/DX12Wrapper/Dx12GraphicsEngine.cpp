#include "Dx12GraphicsEngine.h"

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

#include <DirectXMath.h>

#include <wrl.h>
#include <dshow.h>

using namespace Microsoft::WRL;

namespace DX12Wrapper
{
	// 静的メンバ変数の実体化
	ComPtr<ID3D12Device> Dx12GraphicsEngine::m_device = nullptr;
	ComPtr<ID3D12GraphicsCommandList> Dx12GraphicsEngine::m_cmdList = nullptr;
	ComPtr<ID3D12CommandAllocator> Dx12GraphicsEngine::m_cmdAllocator = nullptr;
	ComPtr<ID3D12CommandQueue> Dx12GraphicsEngine::m_cmdQueue = nullptr;
	ComPtr<IDXGISwapChain3> Dx12GraphicsEngine::m_swapchain = nullptr;
	ComPtr<IDXGIFactory4> Dx12GraphicsEngine::m_dxgiFactory = nullptr;
	ComPtr<ID3D12Fence> Dx12GraphicsEngine::m_fence = nullptr;
	UINT Dx12GraphicsEngine::m_fenceVal = 0;

	HWND Dx12GraphicsEngine::m_hwnd = 0;
	UINT Dx12GraphicsEngine::m_windowWidth = 0;
	UINT Dx12GraphicsEngine::m_windowHeight = 0;

	RenderingContext Dx12GraphicsEngine::m_renderContext;
	RenderTargetBuffer Dx12GraphicsEngine::m_frameBuffers[DOUBLE_BUFFER];
	DescriptorHeapRTV Dx12GraphicsEngine::m_frameHeap;

	DepthStencilBufferData Dx12GraphicsEngine::m_depthStencilBufferData;
	DepthStencilBuffer Dx12GraphicsEngine::m_depthStencilBuffer;
	DescriptorHeapDSV Dx12GraphicsEngine::m_dsvHeap;

	DescriptorHeapCBV_SRV_UAV Dx12GraphicsEngine::m_imguiHeap;

	DirectX::GraphicsMemory* Dx12GraphicsEngine::m_graphicsMemory = nullptr;

	CD3DX12_VIEWPORT Dx12GraphicsEngine::m_viewport;
	CD3DX12_RECT Dx12GraphicsEngine::m_scissorRect;

	Utility::RESULT Dx12GraphicsEngine::Init(
		const HWND& hwnd, const UINT& windowWidth, const UINT& windowHeight)
	{
		assert(windowWidth > 0 && windowHeight > 0);

		// ウィンドウ関連の変数初期化
		m_hwnd = hwnd;
		m_windowWidth = windowWidth;
		m_windowHeight = windowHeight;

		// デバッグレイヤー有効
#ifdef _DEBUG
		if (FAILED(EnableDebugLayer())) { return Utility::RESULT::FAILED; }
#endif // DEBUG

		// デバイスとファクトリー生成
		if (FAILED(CreateDeviceAndDXGIFactory())) { return Utility::RESULT::FAILED; }

		// コマンドアロケータ、リスト、キュー生成
		if (FAILED(CreateCommandX())) { return Utility::RESULT::FAILED; }

		// スワップチェーン生成(ダブルバッファリング用のバッファー生成)
		if (FAILED(CreateSwapChain(hwnd, windowWidth, windowHeight, m_dxgiFactory))) { return Utility::RESULT::FAILED; }

		// フェンス生成
		if (FAILED(CreateFence())) { return Utility::RESULT::FAILED; }

		// フレームバッファ―(最終レンダリング先)生成
		if (CreateFrameRenderTarget() == Utility::RESULT::FAILED) { return Utility::RESULT::FAILED; }

		// Imgui用のヒープ生成
		if (CreateImguiDescriptorHeap() == Utility::RESULT::FAILED) { return Utility::RESULT::FAILED; }

		// レンダリングコンテキストの初期化
		m_renderContext.Init(*m_cmdList.Get());

		// GraphicsMemory初期化(DirectXTKを利用したフォントレンダリングで使用)
		m_graphicsMemory = new DirectX::GraphicsMemory(m_device.Get());

		m_viewport = CD3DX12_VIEWPORT(
			0.f, 0.f, static_cast<float>(windowWidth), static_cast<float>(windowHeight));
		m_scissorRect = CD3DX12_RECT(0, 0, windowWidth, windowHeight);

		return Utility::RESULT::SUCCESS;
	}

	HRESULT Dx12GraphicsEngine::EnableDebugLayer()
	{
		ID3D12Debug* debugLayer = nullptr;
		HRESULT result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));
		debugLayer->EnableDebugLayer();
		debugLayer->Release();
		return result;
	}

	HRESULT Dx12GraphicsEngine::CreateDeviceAndDXGIFactory()
	{
		// フィーチャーレベル列挙
		D3D_FEATURE_LEVEL levels[] = {
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};

		// ファクトリー生成
		HRESULT result = CreateDXGIFactory2(
			DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			MessageBoxA(m_hwnd, "ファクトリー生成失敗", "エラー", MB_OK | MB_ICONERROR);
			return result;
		}

		// アダプター列挙
		std::vector<ComPtr<IDXGIAdapter>> adapters;
		ComPtr<IDXGIAdapter> tmpAdapter = nullptr;
		// EnumAdaptersの第一引数がUINTなので合わせる
		for (UINT i = 0; m_dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++) {
			adapters.push_back(tmpAdapter);
		}

		// "NVIDIA"のGPUを探す
		for (auto adpt : adapters) {
			DXGI_ADAPTER_DESC adesc = {};
			adpt->GetDesc(&adesc);
			std::wstring strDesc = adesc.Description;
			if (strDesc.find(L"NVIDIA") != std::string::npos) {
				tmpAdapter = adpt;
				break;
			}
		}

		// Direct3Dデバイスの初期化
		D3D_FEATURE_LEVEL featureLevel;
		for (auto l : levels) {
			if (SUCCEEDED(
				D3D12CreateDevice(tmpAdapter.Get(), l, IID_PPV_ARGS(m_device.ReleaseAndGetAddressOf())))) {
				featureLevel = l;
				break;
			}
		}

		return result;
	}

	HRESULT Dx12GraphicsEngine::CreateCommandX()
	{
		// コマンドアロケータ生成
		HRESULT result = m_device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_cmdAllocator.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			MessageBoxA(m_hwnd, "コマンドアロケータ生成失敗", "エラー", MB_OK | MB_ICONERROR);
			return result;
		}

		// コマンドリスト生成
		result = m_device->CreateCommandList(
			0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_cmdAllocator.Get(), nullptr,
			IID_PPV_ARGS(m_cmdList.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			MessageBoxA(m_hwnd, "コマンドリスト生成失敗", "エラー", MB_OK | MB_ICONERROR);
			return result;
		}

		// コマンドキュー生成
		D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
		cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		cmdQueueDesc.NodeMask = 0;
		cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		result = m_device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(m_cmdQueue.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			MessageBoxA(m_hwnd, "コマンドキュー生成失敗", "エラー", MB_OK | MB_ICONERROR);
		}

		return result;
	}

	HRESULT Dx12GraphicsEngine::CreateSwapChain(
		const HWND& hwnd, const UINT& windowWidth, const UINT& windowHeight,
		const ComPtr<IDXGIFactory4>& dxgiFactory)
	{
		DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
		swapchainDesc.Width = windowWidth;
		swapchainDesc.Height = windowHeight;
		swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapchainDesc.Stereo = false;
		swapchainDesc.SampleDesc.Count = 1;
		swapchainDesc.SampleDesc.Quality = 0;
		swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
		swapchainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapchainDesc.BufferCount = DOUBLE_BUFFER;
		swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT result = m_dxgiFactory->CreateSwapChainForHwnd(m_cmdQueue.Get(),
			hwnd,
			&swapchainDesc,
			nullptr,
			nullptr,
			(IDXGISwapChain1**)m_swapchain.ReleaseAndGetAddressOf());

		if (FAILED(result)) {
			MessageBoxA(m_hwnd, "スワップチェーン生成失敗", "エラー", MB_OK | MB_ICONERROR);
		}

		return result;
	}

	HRESULT Dx12GraphicsEngine::CreateFence()
	{
		HRESULT result = m_device->CreateFence(
			m_fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			MessageBoxA(m_hwnd, "フェンス生成失敗", "エラー", MB_OK | MB_ICONERROR);
		}

		return result;
	}

	ID3D12Device& Dx12GraphicsEngine::Device()
	{
		return *m_device.Get();
	}

	ID3D12GraphicsCommandList& Dx12GraphicsEngine::CmdList()
	{
		return *m_cmdList.Get();
	}

	ID3D12CommandAllocator& Dx12GraphicsEngine::CmdAllocator()
	{
		return *m_cmdAllocator.Get();
	}

	ID3D12CommandQueue& Dx12GraphicsEngine::CmdQueue()
	{
		return *m_cmdQueue.Get();
	}

	IDXGISwapChain3& Dx12GraphicsEngine::SwapChain()
	{
		return *m_swapchain.Get();
	}

	void Dx12GraphicsEngine::BeginDraw()
	{
		// 描画対象のバッファーを示すインデックス取得
		auto bbIdx = m_swapchain->GetCurrentBackBufferIndex();

		// 描画対象バッファーへ移動
		auto rtvHandle = m_frameHeap.GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIdx * m_frameHeap.GetHandleIncrimentSize();

		// 深度バッファー
		auto dsvHandle = m_dsvHeap.GetCPUDescriptorHandleForHeapStart();

		// バリア処理
		m_renderContext.TransitionResourceState(
			m_frameBuffers[bbIdx].GetBuffer(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET);

		// レンダーターゲットセット
		m_renderContext.SetRenderTarget(&rtvHandle, &dsvHandle);

		// 画面を指定色でクリア
		DirectX::XMFLOAT4 color(0.f, 0.f, 0.f, 1.f);
		m_renderContext.ClearRenderTarget(rtvHandle, color, 0, nullptr);

		// デプスステンシルバッファをクリア
		m_renderContext.ClearDepthStencilView(
			dsvHandle, D3D12_CLEAR_FLAG_DEPTH,
			m_depthStencilBufferData.clearDepth, m_depthStencilBufferData.clearStencil, 0, nullptr);

		// ビューポートとシザー矩形セット
		m_renderContext.SetViewport(m_viewport);
		m_renderContext.SetScissorRect(m_scissorRect);
	}

	void Dx12GraphicsEngine::EndDraw()
	{
		// Imguiの描画
		m_renderContext.SetDescriptorHeap(m_imguiHeap);

		// 描画対象のバッファーを示すインデックス取得
		auto bbIdx = m_swapchain->GetCurrentBackBufferIndex();

		// バリア処理
		m_renderContext.TransitionResourceState(
			m_frameBuffers[bbIdx].GetBuffer(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT);

		// 命令の受付終了
		m_renderContext.Close();

		// コマンドリストの実行
		ID3D12CommandList* cmdlists[] = { m_cmdList.Get() };
		m_cmdQueue->ExecuteCommandLists(1, cmdlists);

		// CPUとGPUの同期
		m_cmdQueue->Signal(m_fence.Get(), ++m_fenceVal);
		if (m_fence->GetCompletedValue() != m_fenceVal) {
			// イベントハンドル取得
			auto event = CreateEvent(nullptr, false, false, nullptr);

			m_fence->SetEventOnCompletion(m_fenceVal, event);

			// イベントが発生するまで待ち続ける
			WaitForSingleObject(event, INFINITE);

			// イベントハンドルを閉じる
			CloseHandle(event);
		}

		m_cmdAllocator->Reset();	                        // キューをクリア
		m_renderContext.Reset(*m_cmdAllocator.Get());	    // コマンドを受け付けられる状態にする

		// フリップ
		m_swapchain->Present(1, 0);

		m_graphicsMemory->Commit(m_cmdQueue.Get());
	}

	void Dx12GraphicsEngine::SetFrameRenderTarget(const CD3DX12_VIEWPORT& viewport, const CD3DX12_RECT& scissorRect)
	{
		// 描画対象のバッファーを示すインデックス取得
		auto bbIdx = m_swapchain->GetCurrentBackBufferIndex();

		// 描画対象バッファーへ移動
		auto rtvHandle = m_frameHeap.GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIdx * m_frameHeap.GetHandleIncrimentSize();

		// 深度バッファー
		auto dsvHandle = m_dsvHeap.GetCPUDescriptorHandleForHeapStart();

		// レンダーターゲットセット
		m_renderContext.SetRenderTarget(&rtvHandle, &dsvHandle);

		// 画面を指定色でクリア
		DirectX::XMFLOAT4 color(0.f, 1.f, 1.f, 1.f);
		m_renderContext.ClearRenderTarget(rtvHandle, color, 0, nullptr);

		// デプスステンシルバッファをクリア
		m_renderContext.ClearDepthStencilView(
			dsvHandle, D3D12_CLEAR_FLAG_DEPTH,
			m_depthStencilBufferData.clearDepth, m_depthStencilBufferData.clearStencil, 0, nullptr);

		// ビューポートとシザー矩形セット
		m_renderContext.SetViewport(viewport);
		m_renderContext.SetScissorRect(scissorRect);
	}

	Utility::RESULT Dx12GraphicsEngine::CreateFrameRenderTarget()
	{
		Utility::RESULT result;

		// ディスクリプタヒープ生成
		m_frameHeap.Create(*m_device.Get());

		// バッファー生成してディスクリプタヒープに登録
		for (size_t idx = 0; idx < DOUBLE_BUFFER; idx++) {
			// 生成
			result = m_frameBuffers[idx].Create(*m_device.Get(), *m_swapchain.Get(), idx);
			if (result == Utility::RESULT::FAILED) { return result; }
			// 登録
			m_frameHeap.RegistDescriptor(*m_device.Get(), m_frameBuffers[idx], DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
		}

		// デプスステンシルバッファー生成
		m_depthStencilBufferData.width = m_windowWidth;
		m_depthStencilBufferData.height = m_windowHeight;
		result = m_depthStencilBuffer.Create(*m_device.Get(), m_depthStencilBufferData);
		if (result == Utility::RESULT::FAILED) { return result; }

		// デプスステンシル用ディスクリプタヒープ生成
		result = m_dsvHeap.Create(*m_device.Get());
		if (result == Utility::RESULT::FAILED) { return result; }

		// デプスステンシルビュー生成
		m_dsvHeap.RegistDescriptor(*m_device.Get(), m_depthStencilBuffer);

		return Utility::RESULT::SUCCESS;
	}

	Utility::RESULT Dx12GraphicsEngine::CreateImguiDescriptorHeap()
	{
		return m_imguiHeap.Create(*m_device.Get());
	}

	DX12Wrapper::RenderingContext& Dx12GraphicsEngine::GetRenderingContext()
	{
		return m_renderContext;
	}
	DX12Wrapper::DescriptorHeapRTV& Dx12GraphicsEngine::GetFrameBufferDescriptorHeap()
	{
		return m_frameHeap;
	}
	const CD3DX12_VIEWPORT& Dx12GraphicsEngine::GetViewport()
	{
		return m_viewport;
	}
	Dx12GraphicsEngine::~Dx12GraphicsEngine()
	{
		if (m_graphicsMemory != nullptr)
		{
			delete m_graphicsMemory;
			m_graphicsMemory = nullptr;
		}
	}
}