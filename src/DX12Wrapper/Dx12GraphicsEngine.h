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
		/// DirectX�ŕ`����s�����߂̏���������
		/// </summary>
		/// <param name="hwnd">�E�B���h�E�n���h��</param>
		/// <param name="windowWidth">�E�B���h�E��</param>
		/// <param name="windowHeight">�E�B���h�E��</param>
		/// <param name="dxgiFactory">�t�@�N�g���[</param>
		/// <returns>Utility::RESULT::SUCCESS: ���� Utility::RESULT::FAILED: ���s</returns>
		static Utility::RESULT Init(const HWND& hwnd,
			const UINT& windowWidth, const UINT& windowHeight,
			const UINT& viewportWidth, const UINT& viewportHeight,
			const UINT& scissorRectWidth, const UINT& scissorRectHeight);

		/// <summary>
		/// �f�o�C�X�擾
		/// </summary>
		/// <returns></returns>
		static ID3D12Device& Device();
		/// <summary>
		/// �R�}���h���X�g�擾
		/// </summary>
		/// <returns></returns>
		static ID3D12GraphicsCommandList& CmdList();
		/// <summary>
		/// �R�}���h�A���P�[�^�擾
		/// </summary>
		/// <returns></returns>
		static ID3D12CommandAllocator& CmdAllocator();
		/// <summary>
		/// �R�}���h�L���[�擾
		/// </summary>
		/// <returns></returns>
		static ID3D12CommandQueue& CmdQueue();
		/// <summary>
		/// �X���b�v�`�F�[���擾
		/// </summary>
		/// <returns></returns>
		static IDXGISwapChain3& SwapChain();

		/// <summary>
		/// 1�t���[���̕`��J�n
		/// </summary>
		static void BeginDraw();
		/// <summary>
		/// 1�t���[���̕`��I��
		/// </summary>
		static void EndDraw();
		/// <summary>
		/// �t���[�������_�[�^�[�Q�b�g�Z�b�g
		/// </summary>
		static void SetFrameRenderTarget(const CD3DX12_VIEWPORT& viewport, const CD3DX12_RECT& scissorRect);

		/// <summary>
		/// �����_�����O�R���e�L�X�g�擾
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

		static constexpr UINT DOUBLE_BUFFER = 2;	// �_�u���o�b�t�@�����O

		// DXGI�֘A
		static Microsoft::WRL::ComPtr<IDXGIFactory4> m_dxgiFactory;
		static Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapchain;

		// DirectX12�������֘A
		static Microsoft::WRL::ComPtr<ID3D12Device> m_device;
		static Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_cmdAllocator;
		static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_cmdList;
		static Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_cmdQueue;

		// �t�F���X�֘A
		static Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
		static UINT m_fenceVal;

		// �E�B���h�E�֘A
		static HWND m_hwnd;
		static UINT m_windowWidth;
		static UINT m_windowHeight;

		/// <summary>
		/// �f�o�b�O���C���[��L���ɂ���
		/// </summary>
		/// <returns></returns>
		static HRESULT EnableDebugLayer();
		/// <summary>
		/// �f�o�C�X�ƃt�@�N�g���[����
		/// </summary>
		/// <returns></returns>
		static HRESULT CreateDeviceAndDXGIFactory();
		/// <summary>
		/// �R�}���h�A���P�[�^�A���X�g�A�L���[����
		/// </summary>
		/// <returns></returns>
		static HRESULT CreateCommandX();
		/// <summary>
		/// �X���b�v�`�F�[������
		/// </summary>
		/// <param name="hwnd">�E�B���h�E�n���h��</param>
		/// <param name="windowWidth">�E�B���h�E��</param>
		/// <param name="windowHeightconst">�E�B���h�E��</param>
		/// <param name="dxgiFactory">�t�@�N�g���[</param>
		/// <returns></returns>
		static HRESULT CreateSwapChain(
			const HWND& hwnd, const UINT& windowWidth, const UINT& windowHeight,
			const Microsoft::WRL::ComPtr<IDXGIFactory4>& dxgiFactory);
		/// <summary>
		/// �t�F���X����
		/// </summary>
		/// <returns></returns>
		static HRESULT CreateFence();

		static DX12Wrapper::RenderingContext m_renderContext;	            // �����_�����O�R���e�L�X�g
		static DX12Wrapper::RenderTargetBuffer m_frameBuffers[2];	        // �t���[���o�b�t�@
		static DX12Wrapper::DescriptorHeapRTV m_frameHeap;	                // �t���[���o�b�t�@�p�f�B�X�N���v�^�q�[�v	

		static DX12Wrapper::DepthStencilBufferData m_depthStencilBufferData;	// �f�v�X�X�e���V���o�b�t�@�[�̐ݒ�
		static DX12Wrapper::DepthStencilBuffer m_depthStencilBuffer;		    // �f�v�X�X�e���V���o�b�t�@�[
		static DX12Wrapper::DescriptorHeapDSV m_dsvHeap;					    // �f�v�X�X�e���V���p�q�[�v

		static DX12Wrapper::DescriptorHeapCBV_SRV_UAV m_imguiHeap;

		// �t�H���g�����_�����O�֘A
		static DirectX::GraphicsMemory* m_graphicsMemory;

		static CD3DX12_VIEWPORT m_viewport;
		static CD3DX12_RECT m_scissorRect;

		/// <summary>
		/// �t���[���o�b�t�@�p�̃����_�[�^�[�Q�b�g����
		/// </summary>
		/// <returns></returns>
		static Utility::RESULT CreateFrameRenderTarget();

		static Utility::RESULT CreateImguiDescriptorHeap();

		static DescriptorHeapCBV_SRV_UAV& GetImguiDescriptorHeap();
	};
}