#pragma once

#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "../Utility/EngineUtility.h"


namespace DX12Wrapper
{
	class Texture;
	class ConstantBuffer;
	class UnorderedAccessResource;

	struct ShaderResourceViewDesc;

	/// <summary>
	/// コンスタントバッファー、シェーダーリソース、アンオーダーアクセスリソースの
	/// ディスクリプタヒープのラッパークラス
	/// </summary>
	class DescriptorHeapCBV_SRV_UAV
	{
	public:
		// SpriteのDescriptorHeapのデストラクタでエラー出てるっぽい
		DescriptorHeapCBV_SRV_UAV() = default;
		~DescriptorHeapCBV_SRV_UAV() = default;
		static constexpr int NEXT_REGISTER = -1;

		/// <summary>
		/// ディスクリプタヒープ生成
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <returns></returns>
		Utility::RESULT Create(ID3D12Device& device);

		/// <summary>
		/// GPUのディスクリプタヒープの先頭ハンドルを取得
		/// </summary>
		/// <returns></returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStart() {
			return m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();
		}

		/// <summary>
		/// CPUのディスクリプタヒープの先頭ハンドル取得
		/// </summary>
		/// <returns></returns>
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForHeapStart() {
			return m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
		}

		/// <summary>
		/// GPUのCBV部分のディスクリプタヒープの先頭ハンドルを取得
		/// </summary>
		/// <returns>GPUのCBV部分のディスクリプタヒープの先頭ハンドル</returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStartCBV() {
			return m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();
		}

		/// <summary>
		/// GPUのSRV部分のディスクリプタヒープの先頭ハンドルを取得
		/// </summary>
		/// <returns>GPUのSRV部分のディスクリプタヒープの先頭ハンドル</returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStartSRV() {
			auto gpuHandle = m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();
			gpuHandle.ptr += m_handleIncrimentSize * m_MAX_CBV_DESCRIPTOR_NUM;
			auto cpuHandle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
			cpuHandle.ptr += m_handleIncrimentSize * m_MAX_CBV_DESCRIPTOR_NUM;

			return std::move(gpuHandle);
		}

		/// <summary>
		/// 指定インデックスのSRVハンドルを取得
		/// </summary>
		/// <param name="index">インデックス</param>
		/// <returns>指定インデックスのSRVハンドル</returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandle(int index) {
			auto gpuHandle = m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();
			gpuHandle.ptr += m_handleIncrimentSize * m_MAX_CBV_DESCRIPTOR_NUM;
			gpuHandle.ptr += m_handleIncrimentSize * index;
			auto cpuHandle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
			cpuHandle.ptr += m_handleIncrimentSize * m_MAX_CBV_DESCRIPTOR_NUM;
			cpuHandle.ptr += m_handleIncrimentSize * index;

			return std::move(gpuHandle);
		}

		/// <summary>
		/// GPUハンドル取得
		/// </summary>
		/// <returns>GPUハンドル</returns>
		CD3DX12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() {
			auto gpuHandle = m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();
			gpuHandle.ptr += m_handleIncrimentSize * m_MAX_CBV_DESCRIPTOR_NUM;
			auto cpuHandle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
			cpuHandle.ptr += m_handleIncrimentSize * m_MAX_CBV_DESCRIPTOR_NUM;

			return CD3DX12_GPU_DESCRIPTOR_HANDLE(gpuHandle, m_MAX_CBV_DESCRIPTOR_NUM, static_cast<UINT>(m_handleIncrimentSize));
		}

		/// <summary>
		/// GPUのUAV部分のディスクリプタヒープの先頭ハンドルを取得
		/// </summary>
		/// <returns>GPUのUAV部分のディスクリプタヒープの先頭ハンドル</returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStartUAV() {
			auto handle = m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();
			handle.ptr += m_handleIncrimentSize * (m_MAX_CBV_DESCRIPTOR_NUM + m_MAX_UAV_DESCRIPTOR_NUM);

			return handle;
		}

		/// <summary>
		/// ディスクリプタヒープハンドルのインクリメントサイズ取得
		/// </summary>
		/// <returns>インクリメントサイズ</returns>
		SIZE_T GetHandleIncrimentSize() {
			return m_handleIncrimentSize;
		}

		/// <summary>
		/// コンスタントバッファ―登録
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <param name="constantBuffer">コンスタントバッファ―</param>
		/// <param name="registerNo">
		/// 登録するレジスタ番号 デフォルト値:NEXT_REGISTER(-1) 現在登録されているリソース数の次のレジスタを使用
		/// </param>
		void RegistConstantBuffer(
			ID3D12Device& device, ConstantBuffer& constantBuffer, const int& registerNo = NEXT_REGISTER);

		/// <summary>
		/// シェーダーリソース(テクスチャ)登録
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <param name="texture">テクスチャ</param>
		/// <param name="registerNo">
		/// 登録するレジスタ番号 デフォルト値:NEXT_REGISTER(-1) 現在登録されているリソース数の次のレジスタを使用
		/// </param>
		int RegistShaderResource(
			ID3D12Device& device, Texture& texture, ShaderResourceViewDesc& desc, const int& registerNo = NEXT_REGISTER);

		/// <summary>
		/// アンオーダーアクセスリソース登録
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <param name="unorderedAccessResource">アンオーダーアクセスリソース</param>
		/// <param name="registerNo">
		/// 登録するレジスタ番号 デフォルト値:NEXT_REGISTER(-1) 現在登録されているリソース数の次のレジスタを使用
		/// </param>
		void RegistUnorderedAccessResource(
			ID3D12Device& device, UnorderedAccessResource& unorderedAccessResource,
			const int& registerNo = NEXT_REGISTER);

		/// <summary>
		/// コンスタントバッファーが登録されているか
		/// </summary>
		/// <returns></returns>
		bool IsRegistedConstantBuffer() { return m_registedCBVNum > 0; }

		/// <summary>
		/// シェーダーリソースが登録されているか
		/// </summary>
		/// <returns></returns>
		bool IsRegistedShaderResource() { return m_registedSRVNum > 0; }

		/// <summary>
		/// アンオーダーアクセスリソースが登録されているか
		/// </summary>
		/// <returns></returns>
		bool IsRegistedUnorderedAccessResource() { return m_registedUAVNum > 0; }

		/// <summary>
		/// ディスクリプタヒープのアドレス取得
		/// </summary>
		/// <returns>ディスクリプタヒープのアドレス</returns>
		ID3D12DescriptorHeap** GetDescriptorHeapAddress() {
			return m_descriptorHeap.GetAddressOf();
		}

	private:
		static constexpr UINT m_MAX_CBV_DESCRIPTOR_NUM = 64;	// 定数バッファーの最大数
		static constexpr UINT m_MAX_SRV_DESCRIPTOR_NUM = 64;	// シェーダーリソースの最大数
		static constexpr UINT m_MAX_UAV_DESCRIPTOR_NUM = 64;	// アンオーダーアクセスの最大数

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_descriptorHeap = nullptr;

		SIZE_T m_handleIncrimentSize = 0;
		UINT m_registedCBVNum = 0;
		UINT m_registedSRVNum = 0;
		UINT m_registedUAVNum = 0;

		/// <summary>
		/// ディスクリプタヒープ生成
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <returns></returns>
		HRESULT CreateDescriptorHeap(ID3D12Device& device);
	};
}