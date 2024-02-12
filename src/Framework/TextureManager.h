#pragma once

#include "DX12Wrapper/Texture.h"

namespace Framework
{
	/// <summary>
	/// テクスチャを管理するクラス
	/// </summary>
	class TextureManager
	{
	public:
		TextureManager() = default;
		~TextureManager() = default;
		
		/// <summary>
		/// テクスチャを読み込む
		/// 既に読み込まれている場合はキャッシュから取得する
		/// 読み込まれていない場合は新規に読み込む
		/// </summary>
		/// <param name="filePath">テクスチャへのファイルパス</param>
		/// <returns>テクスチャ</returns>
		static std::shared_ptr<DX12Wrapper::Texture> LoadTexture(std::wstring filePath);

	private:
		// テクスチャのファイルパスとテクスチャのキャッシュ
		static std::unordered_map<std::wstring, std::shared_ptr<DX12Wrapper::Texture>> m_sprites;
	};
}