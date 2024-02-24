#pragma once
#include "IComponent.h"

#include <d3d12.h>
#include <DirectXMath.h>

namespace DX12Wrapper
{
	class Texture;
	class VertexBuffer;
	class IndexBuffer;
	class DescriptorHeapCBV_SRV_UAV;
}

namespace Framework
{
	/// <summary>
	/// スプライトの原点位置の種類
	/// </summary>
	enum class SPRITE_PIVOT
	{
		TOP_LEFT,
		CENTER,
	};

	/// <summary>
	/// 画像描画の為に必要なデータを持つクラス
	/// </summary>
	class Sprite
	{
	public:
		Sprite(const std::wstring& path, SPRITE_PIVOT pivot = SPRITE_PIVOT::CENTER);
		~Sprite();

		/// <summary>
		/// テクスチャを読み込む
		/// キャッシュされている場合はキャッシュから取得する
		/// そうでない場合は新規に読み込む
		/// </summary>
		/// <param name="path">テクスチャへのパス</param>
		void LoadTexture(const std::wstring& path);

		/// <summary>
		/// テクスチャをセットする
		/// 既にセットされている場合は上書きする
		/// </summary>
		/// <param name="texture">テクスチャ</param>
		void SetTexture(std::shared_ptr<DX12Wrapper::Texture>& texture);

		DX12Wrapper::Texture& GetTexture() const;
		DX12Wrapper::DescriptorHeapCBV_SRV_UAV& GetDescriptorHeap() const;
		SPRITE_PIVOT GetPivot() const;

	private:
		// 複数のオブジェクトで同じテクスチャを使う場合を考慮
		std::shared_ptr<DX12Wrapper::Texture> m_texture = nullptr;
		std::unique_ptr<DX12Wrapper::DescriptorHeapCBV_SRV_UAV> m_descriptorHeap = nullptr;
		SPRITE_PIVOT m_pivot;
	};
}