#include "pch.h"
#include "TextureManager.h"

namespace Framework
{
	std::unordered_map<std::wstring, std::shared_ptr<DX12Wrapper::Texture>> TextureManager::m_sprites;

	std::shared_ptr<DX12Wrapper::Texture> TextureManager::LoadTexture(std::wstring filePath)
	{
		auto itr = m_sprites.find(filePath);
		if (itr != m_sprites.end())
		{
			return itr->second;
		}

		std::shared_ptr<DX12Wrapper::Texture> texture = std::make_shared<DX12Wrapper::Texture>();
		texture->CreateTextureFromWIC(filePath);

		m_sprites[filePath] = texture;

		return texture;
	}
}