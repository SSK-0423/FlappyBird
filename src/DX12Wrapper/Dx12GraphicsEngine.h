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
	private:
		Dx12GraphicsEngine();
		~Dx12GraphicsEngine() = default;
		Dx12GraphicsEngine(const Dx12GraphicsEngine& inst) = delete;
		void operator=(const Dx12GraphicsEngine& inst) = delete;

		static constexpr UINT DOUBLE_BUFFER = 2;	// �_�u���o�b�t�@�����O

	public:
		/// <summary>
		/// �N���X�̃V���O���g���C���X�^���X�擾
		/// </summary>
		/// <returns>�V���O���g���C���X�^���X</returns>
		static Dx12GraphicsEngine& Instance();

		/// <summary>
		/// DirectX�ŕ`����s�����߂̏���������
		/// </summary>
		/// <param name="hwnd">�E�B���h�E�n���h��</param>
		/// <param name="windowWidth">�E�B���h�E��</param>
		/// <param name="windowHeight">�E�B���h�E��</param>
		/// <param name="dxgiFactory">�t�@�N�g���[</param>
		/// <returns>Utility::RESULT::SUCCESS: ���� Utility::RESULT::FAILED: ���s</returns>
		Utility::RESULT Init(const HWND& hwnd, const UINT& windowWidth, const UINT& windowHeight);

	private:
		// DXGI�֘A
		Microsoft::WRL::ComPtr<IDXGIFactory4> _dxgiFactory = nullptr;
		Microsoft::WRL::ComPtr<IDXGISwapChain3> _swapchain = nullptr;

		// DirectX12�������֘A
		Microsoft::WRL::ComPtr<ID3D12Device> _device = nullptr;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _cmdAllocator = nullptr;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> _cmdList = nullptr;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> _cmdQueue = nullptr;

		// �t�F���X�֘A
		Microsoft::WRL::ComPtr<ID3D12Fence> _fence = nullptr;
		UINT _fenceVal = 0;

		// �E�B���h�E�֘A
		HWND _hwnd;
		UINT _windowWidth;
		UINT _windowHeight;

		/// <summary>
		/// �f�o�b�O���C���[��L���ɂ���
		/// </summary>
		/// <returns></returns>
		HRESULT EnableDebugLayer();
		/// <summary>
		/// �f�o�C�X�ƃt�@�N�g���[����
		/// </summary>
		/// <returns></returns>
		HRESULT CreateDeviceAndDXGIFactory();
		/// <summary>
		/// �R�}���h�A���P�[�^�A���X�g�A�L���[����
		/// </summary>
		/// <returns></returns>
		HRESULT CreateCommandX();
		/// <summary>
		/// �X���b�v�`�F�[������
		/// </summary>
		/// <param name="hwnd">�E�B���h�E�n���h��</param>
		/// <param name="windowWidth">�E�B���h�E��</param>
		/// <param name="windowHeightconst">�E�B���h�E��</param>
		/// <param name="dxgiFactory">�t�@�N�g���[</param>
		/// <returns></returns>
		HRESULT CreateSwapChain(
			const HWND& hwnd, const UINT& windowWidth, const UINT& windowHeight,
			const Microsoft::WRL::ComPtr<IDXGIFactory4>& dxgiFactory);
		/// <summary>
		/// �t�F���X����
		/// </summary>
		/// <returns></returns>
		HRESULT CreateFence();

	public:
		/// <summary>
		/// �f�o�C�X�擾
		/// </summary>
		/// <returns></returns>
		ID3D12Device& Device();
		/// <summary>
		/// �R�}���h���X�g�擾
		/// </summary>
		/// <returns></returns>
		ID3D12GraphicsCommandList& CmdList();
		/// <summary>
		/// �R�}���h�A���P�[�^�擾
		/// </summary>
		/// <returns></returns>
		ID3D12CommandAllocator& CmdAllocator();
		/// <summary>
		/// �R�}���h�L���[�擾
		/// </summary>
		/// <returns></returns>
		ID3D12CommandQueue& CmdQueue();
		/// <summary>
		/// �X���b�v�`�F�[���擾
		/// </summary>
		/// <returns></returns>
		IDXGISwapChain3& SwapChain();

		/// <summary>
		/// 1�t���[���̕`��J�n
		/// </summary>
		void BeginDraw();
		/// <summary>
		/// 1�t���[���̕`��I��
		/// </summary>
		void EndDraw();
		/// <summary>
		/// �t���[�������_�[�^�[�Q�b�g�Z�b�g
		/// </summary>
		void SetFrameRenderTarget(const CD3DX12_VIEWPORT& viewport, const CD3DX12_RECT& scissorRect);

	private:
		DX12Wrapper::RenderingContext m_renderContext;	            // �����_�����O�R���e�L�X�g
		DX12Wrapper::RenderTargetBuffer _frameBuffers[2];	        // �t���[���o�b�t�@
		DX12Wrapper::DescriptorHeapRTV _frameHeap;	                // �t���[���o�b�t�@�p�f�B�X�N���v�^�q�[�v	

		DX12Wrapper::DepthStencilBufferData depthStencilBufferData;	// �f�v�X�X�e���V���o�b�t�@�[�̐ݒ�
		DX12Wrapper::DepthStencilBuffer _depthStencilBuffer;		    // �f�v�X�X�e���V���o�b�t�@�[
		DX12Wrapper::DescriptorHeapDSV _dsvHeap;					    // �f�v�X�X�e���V���p�q�[�v

		DX12Wrapper::DescriptorHeapCBV_SRV_UAV m_imguiHeap;

		// �t�H���g�����_�����O�֘A
		std::unique_ptr<DirectX::GraphicsMemory> m_graphicsMemory;

		CD3DX12_VIEWPORT m_viewport;
		CD3DX12_RECT m_scissorRect;

		/// <summary>
		/// �t���[���o�b�t�@�p�̃����_�[�^�[�Q�b�g����
		/// </summary>
		/// <returns></returns>
		Utility::RESULT CreateFrameRenderTarget();

	public:
		/// <summary>
		/// �����_�����O�R���e�L�X�g�擾
		/// </summary>
		/// <returns></returns>
		DX12Wrapper::RenderingContext& GetRenderingContext();

		DX12Wrapper::DescriptorHeapRTV& GetFrameBufferDescriptorHeap();

		const CD3DX12_VIEWPORT& GetViewport();
	};
}