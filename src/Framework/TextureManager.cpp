#include "pch.h"
#include "TextureManager.h"

namespace Framework
{
	// 静的メンバ変数の実体化
	std::unordered_map<std::wstring, std::shared_ptr<DX12Wrapper::Texture>> TextureManager::m_sprites;

	std::shared_ptr<DX12Wrapper::Texture> TextureManager::LoadTexture(std::wstring filePath)
	{
		// 既に読み込まれている場合はキャッシュから取得する
		auto itr = m_sprites.find(filePath);
		if (itr != m_sprites.end())
		{
			return itr->second;
		}

		// 読み込まれていない場合は新規に読み込む
		std::shared_ptr<DX12Wrapper::Texture> texture = std::make_shared<DX12Wrapper::Texture>();
		texture->CreateTextureFromWIC(filePath);

		m_sprites[filePath] = texture;

		return texture;
	}
}