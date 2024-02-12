#pragma once

#include <d3d12.h>
#include <DirectXTex.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"DirectXTex.lib")
#pragma comment(lib,"dxguid.lib")

#include "Dx12GraphicsEngine.h"

#include "../Utility/EngineUtility.h"

namespace DX12Wrapper
{
	class RenderTargetBuffer;
	class DepthStencilBuffer;

	/// <summary>
	/// シェーダーリソースをテクスチャとして扱うためのクラス
	/// </summary>
	class Texture {
	public:
		Texture() = default;
		~Texture() = default;

		Texture(const Texture& inst);
		Texture& operator=(const Texture& inst);

		// std::unordered_mapのキーとして使用するため
		bool operator==(const Texture& inst) const;

		friend std::hash<Texture>;

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> m_uploadBuffer = nullptr;		// 中間バッファー(アップロード元)
		Microsoft::WRL::ComPtr<ID3D12Resource> m_textureBuffer = nullptr;	// テクスチャバッファー(アップロード先)

		std::vector<D3D12_SUBRESOURCE_DATA> m_subresources; // キューブテクスチャ用サブリソース

		// ScratchImage::GetImageの戻り値にconstがついているため
		const DirectX::Image* m_image = nullptr;	// テクスチャの生データ
		DirectX::TexMetadata m_metaData;	        // テクスチャのメタ情報
		DirectX::ScratchImage m_scratchImage;    // 

		/// <summary>
		/// WIC対応のファイル(bmp,png,jpeg,tiffなど)を読み込む
		/// </summary>
		/// <param name="texPath">テクスチャへのパス</param>
		/// <returns></returns>
		HRESULT LoadTextureFromWICFile(const std::wstring& texturePath);

		/// <summary>
		/// DDSファイル読み込み
		/// </summary>
		/// <param name="texturePath">テクスチャへのパス</param>
		/// <returns></returns>
		HRESULT LoadTextureFromDDSFile(const std::wstring& texturePath);

		/// <summary>
		/// 中間バッファーとテクスチャバッファー生成
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <returns></returns>
		HRESULT CreateUploadAndTextureBuffer(ID3D12Device& device);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="device"></param>
		/// <returns></returns>
		HRESULT CreateUploadAndCubeTextureBuffer(ID3D12Device& device);

		/// <summary>
		/// マップ処理
		/// </summary>
		/// <returns></returns>
		HRESULT MapTexture();

		/// <summary>
		/// アップロードバッファーの内容をテクスチャバッファーへコピー
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <param name="graphicsEngine">グラフィクスエンジン</param>
		/// <returns></returns>
		HRESULT CopyTexture(ID3D12Device& device);

		/// <summary>
		/// アップロードバッファーの内容をキューブテクスチャバッファーへコピー
		/// </summary>
		/// <returns></returns>
		HRESULT CopyCubeTexture();

		/// <summary>
		/// RGBA等のデ―タを画像ではなく直接セットする
		/// </summary>
		/// <param name="data"></param>
		/// <param name="stride"></param>
		/// <param name="dataNum"></param>
		/// <param name="width"></param>
		/// <param name="height"></param>
		/// <param name="format"></param>
		void SetTextureData(
			uint8_t* data, const size_t& stride, const size_t& dataNum, const size_t& width, const size_t& height, const DXGI_FORMAT& format);

	public:
		/// <summary>
		/// WIC対応のファイル((bmp,png,jpeg,tiffなど)からテクスチャを生成
		/// </summary>
		/// <param name="graphicsEngine">グラフィクスエンジン</param>
		/// <param name="texturePath">テクスチャへのパス</param>
		/// <returns>成功：Utility::RESULT::SUCCESS 失敗：Utility::RESULT::FAILED</returns>
		Utility::RESULT CreateTextureFromWIC(const std::wstring& texturePath);

		/// <summary>
		/// DDSファイルからテクスチャを生成
		/// </summary>
		/// <param name="graphicsEngine">グラフィクスエンジン</param>
		/// <param name="texturePath">テクスチャへのパス</param>
		/// <returns>成功：Utility::RESULT::SUCCESS 失敗：Utility::RESULT::FAILED</returns>
		Utility::RESULT CreateTextureFromDDS(const std::wstring& texturePath);

		/// <summary>
		/// 用意したRGBAデータからテクスチャ生成
		/// </summary>
		/// <param name="graphicsEngine"></param>
		/// <param name="data"></param>
		/// <param name="width"></param>
		/// <param name="height"></param>
		/// <returns></returns>
		Utility::RESULT CreateTextureFromConstantData(
			uint8_t* data, const size_t& stride, const size_t& dataNum,
			const size_t& width, const size_t& height, const DXGI_FORMAT& format);

		/// <summary>
		/// レンダーターゲットからテクスチャ生成
		/// </summary>
		/// <param name="renderTargetBuffer">レンダーターゲットバッファー</param>
		void CreateTextureFromRenderTarget(RenderTargetBuffer& renderTargetBuffer);

		/// <summary>
		/// デプスステンシルバッファーからテクスチャ生成
		/// </summary>
		/// <param name="depthStencilBuffer">デプスステンシルバッファー</param>
		void CreateTextureFromDepthStencil(DepthStencilBuffer& depthStencilBuffer);

		Utility::RESULT CreateCubeTextureFromDDS(Dx12GraphicsEngine& graphicsEngine, const std::wstring& texturePath);

		/// <summary>
		/// テクスチャバッファー取得
		/// </summary>
		/// <returns>テクスチャバッファー</returns>
		ID3D12Resource& GetBuffer() const {
			return *m_textureBuffer.Get();
		}

		/// <summary>
		/// テクスチャのメタデータ取得
		/// </summary>
		/// <returns>テクスチャのメタデータ</returns>
		DirectX::TexMetadata& GetTexMetadata() {
			return m_metaData;
		}

		/// <summary>
		/// テクスチャの生データ取得
		/// </summary>
		/// <returns>テクスチャの生データ</returns>
		const DirectX::Image& GetImage() {
			return *m_image;
		}
	};
}

// std::unordered_mapのキーとして使用するためにハッシュ関数を定義
namespace std
{
	template<>
	class hash<DX12Wrapper::Texture>
	{
	public:
		std::size_t operator()(const DX12Wrapper::Texture& inst) const
		{
			return std::hash<size_t>()((size_t)inst.m_textureBuffer.Get());
		}
	};
}