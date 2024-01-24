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
	// �ÓI�����o�ϐ��̎��̉�
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

		// �E�B���h�E�֘A�̕ϐ�������
		m_hwnd = hwnd;
		m_windowWidth = windowWidth;
		m_windowHeight = windowHeight;

		// �f�o�b�O���C���[�L��
#ifdef _DEBUG
		if (FAILED(EnableDebugLayer())) { return Utility::RESULT::FAILED; }
#endif // DEBUG

		// �f�o�C�X�ƃt�@�N�g���[����
		if (FAILED(CreateDeviceAndDXGIFactory())) { return Utility::RESULT::FAILED; }

		// �R�}���h�A���P�[�^�A���X�g�A�L���[����
		if (FAILED(CreateCommandX())) { return Utility::RESULT::FAILED; }

		// �X���b�v�`�F�[������(�_�u���o�b�t�@�����O�p�̃o�b�t�@�[����)
		if (FAILED(CreateSwapChain(hwnd, windowWidth, windowHeight, m_dxgiFactory))) { return Utility::RESULT::FAILED; }

		// �t�F���X����
		if (FAILED(CreateFence())) { return Utility::RESULT::FAILED; }

		// �t���[���o�b�t�@�\(�ŏI�����_�����O��)����
		if (CreateFrameRenderTarget() == Utility::RESULT::FAILED) { return Utility::RESULT::FAILED; }

		// Imgui�p�̃q�[�v����
		if (CreateImguiDescriptorHeap() == Utility::RESULT::FAILED) { return Utility::RESULT::FAILED; }

		// �����_�����O�R���e�L�X�g�̏�����
		m_renderContext.Init(*m_cmdList.Get());

		// GraphicsMemory������(DirectXTK�𗘗p�����t�H���g�����_�����O�Ŏg�p)
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
		// �t�B�[�`���[���x����
		D3D_FEATURE_LEVEL levels[] = {
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};

		// �t�@�N�g���[����
		HRESULT result = CreateDXGIFactory2(
			DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			MessageBoxA(m_hwnd, "�t�@�N�g���[�������s", "�G���[", MB_OK | MB_ICONERROR);
			return result;
		}

		// �A�_�v�^�[��
		std::vector<ComPtr<IDXGIAdapter>> adapters;
		ComPtr<IDXGIAdapter> tmpAdapter = nullptr;
		// EnumAdapters�̑�������UINT�Ȃ̂ō��킹��
		for (UINT i = 0; m_dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++) {
			adapters.push_back(tmpAdapter);
		}

		// "NVIDIA"��GPU��T��
		for (auto adpt : adapters) {
			DXGI_ADAPTER_DESC adesc = {};
			adpt->GetDesc(&adesc);
			std::wstring strDesc = adesc.Description;
			if (strDesc.find(L"NVIDIA") != std::string::npos) {
				tmpAdapter = adpt;
				break;
			}
		}

		// Direct3D�f�o�C�X�̏�����
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
		// �R�}���h�A���P�[�^����
		HRESULT result = m_device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_cmdAllocator.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			MessageBoxA(m_hwnd, "�R�}���h�A���P�[�^�������s", "�G���[", MB_OK | MB_ICONERROR);
			return result;
		}

		// �R�}���h���X�g����
		result = m_device->CreateCommandList(
			0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_cmdAllocator.Get(), nullptr,
			IID_PPV_ARGS(m_cmdList.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			MessageBoxA(m_hwnd, "�R�}���h���X�g�������s", "�G���[", MB_OK | MB_ICONERROR);
			return result;
		}

		// �R�}���h�L���[����
		D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
		cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		cmdQueueDesc.NodeMask = 0;
		cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		result = m_device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(m_cmdQueue.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			MessageBoxA(m_hwnd, "�R�}���h�L���[�������s", "�G���[", MB_OK | MB_ICONERROR);
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
			MessageBoxA(m_hwnd, "�X���b�v�`�F�[���������s", "�G���[", MB_OK | MB_ICONERROR);
		}

		return result;
	}

	HRESULT Dx12GraphicsEngine::CreateFence()
	{
		HRESULT result = m_device->CreateFence(
			m_fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			MessageBoxA(m_hwnd, "�t�F���X�������s", "�G���[", MB_OK | MB_ICONERROR);
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
		// �`��Ώۂ̃o�b�t�@�[�������C���f�b�N�X�擾
		auto bbIdx = m_swapchain->GetCurrentBackBufferIndex();

		// �`��Ώۃo�b�t�@�[�ֈړ�
		auto rtvHandle = m_frameHeap.GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIdx * m_frameHeap.GetHandleIncrimentSize();

		// �[�x�o�b�t�@�[
		auto dsvHandle = m_dsvHeap.GetCPUDescriptorHandleForHeapStart();

		// �o���A����
		m_renderContext.TransitionResourceState(
			m_frameBuffers[bbIdx].GetBuffer(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET);

		// �����_�[�^�[�Q�b�g�Z�b�g
		m_renderContext.SetRenderTarget(&rtvHandle, &dsvHandle);

		// ��ʂ��w��F�ŃN���A
		DirectX::XMFLOAT4 color(0.f, 0.f, 0.f, 1.f);
		m_renderContext.ClearRenderTarget(rtvHandle, color, 0, nullptr);

		// �f�v�X�X�e���V���o�b�t�@���N���A
		m_renderContext.ClearDepthStencilView(
			dsvHandle, D3D12_CLEAR_FLAG_DEPTH,
			m_depthStencilBufferData.clearDepth, m_depthStencilBufferData.clearStencil, 0, nullptr);

		// �r���[�|�[�g�ƃV�U�[��`�Z�b�g
		m_renderContext.SetViewport(m_viewport);
		m_renderContext.SetScissorRect(m_scissorRect);
	}

	void Dx12GraphicsEngine::EndDraw()
	{
		// Imgui�̕`��
		m_renderContext.SetDescriptorHeap(m_imguiHeap);

		// �`��Ώۂ̃o�b�t�@�[�������C���f�b�N�X�擾
		auto bbIdx = m_swapchain->GetCurrentBackBufferIndex();

		// �o���A����
		m_renderContext.TransitionResourceState(
			m_frameBuffers[bbIdx].GetBuffer(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT);

		// ���߂̎�t�I��
		m_renderContext.Close();

		// �R�}���h���X�g�̎��s
		ID3D12CommandList* cmdlists[] = { m_cmdList.Get() };
		m_cmdQueue->ExecuteCommandLists(1, cmdlists);

		// CPU��GPU�̓���
		m_cmdQueue->Signal(m_fence.Get(), ++m_fenceVal);
		if (m_fence->GetCompletedValue() != m_fenceVal) {
			// �C�x���g�n���h���擾
			auto event = CreateEvent(nullptr, false, false, nullptr);

			m_fence->SetEventOnCompletion(m_fenceVal, event);

			// �C�x���g����������܂ő҂�������
			WaitForSingleObject(event, INFINITE);

			// �C�x���g�n���h�������
			CloseHandle(event);
		}

		m_cmdAllocator->Reset();	                        // �L���[���N���A
		m_renderContext.Reset(*m_cmdAllocator.Get());	    // �R�}���h���󂯕t�������Ԃɂ���

		// �t���b�v
		m_swapchain->Present(1, 0);

		m_graphicsMemory->Commit(m_cmdQueue.Get());
	}

	void Dx12GraphicsEngine::SetFrameRenderTarget(const CD3DX12_VIEWPORT& viewport, const CD3DX12_RECT& scissorRect)
	{
		// �`��Ώۂ̃o�b�t�@�[�������C���f�b�N�X�擾
		auto bbIdx = m_swapchain->GetCurrentBackBufferIndex();

		// �`��Ώۃo�b�t�@�[�ֈړ�
		auto rtvHandle = m_frameHeap.GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIdx * m_frameHeap.GetHandleIncrimentSize();

		// �[�x�o�b�t�@�[
		auto dsvHandle = m_dsvHeap.GetCPUDescriptorHandleForHeapStart();

		// �����_�[�^�[�Q�b�g�Z�b�g
		m_renderContext.SetRenderTarget(&rtvHandle, &dsvHandle);

		// ��ʂ��w��F�ŃN���A
		DirectX::XMFLOAT4 color(0.f, 1.f, 1.f, 1.f);
		m_renderContext.ClearRenderTarget(rtvHandle, color, 0, nullptr);

		// �f�v�X�X�e���V���o�b�t�@���N���A
		m_renderContext.ClearDepthStencilView(
			dsvHandle, D3D12_CLEAR_FLAG_DEPTH,
			m_depthStencilBufferData.clearDepth, m_depthStencilBufferData.clearStencil, 0, nullptr);

		// �r���[�|�[�g�ƃV�U�[��`�Z�b�g
		m_renderContext.SetViewport(viewport);
		m_renderContext.SetScissorRect(scissorRect);
	}

	Utility::RESULT Dx12GraphicsEngine::CreateFrameRenderTarget()
	{
		Utility::RESULT result;

		// �f�B�X�N���v�^�q�[�v����
		m_frameHeap.Create(*m_device.Get());

		// �o�b�t�@�[�������ăf�B�X�N���v�^�q�[�v�ɓo�^
		for (size_t idx = 0; idx < DOUBLE_BUFFER; idx++) {
			// ����
			result = m_frameBuffers[idx].Create(*m_device.Get(), *m_swapchain.Get(), idx);
			if (result == Utility::RESULT::FAILED) { return result; }
			// �o�^
			m_frameHeap.RegistDescriptor(*m_device.Get(), m_frameBuffers[idx], DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
		}

		// �f�v�X�X�e���V���o�b�t�@�[����
		m_depthStencilBufferData.width = m_windowWidth;
		m_depthStencilBufferData.height = m_windowHeight;
		result = m_depthStencilBuffer.Create(*m_device.Get(), m_depthStencilBufferData);
		if (result == Utility::RESULT::FAILED) { return result; }

		// �f�v�X�X�e���V���p�f�B�X�N���v�^�q�[�v����
		result = m_dsvHeap.Create(*m_device.Get());
		if (result == Utility::RESULT::FAILED) { return result; }

		// �f�v�X�X�e���V���r���[����
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