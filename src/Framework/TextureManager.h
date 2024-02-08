#pragma once

#include "DX12Wrapper/Texture.h"

namespace Framework
{
	class TextureManager
	{
	public:
		TextureManager() = default;
		~TextureManager() = default;
		
		static std::shared_ptr<DX12Wrapper::Texture> LoadTexture(std::wstring filePath);

	private:
		static std::unordered_map<std::wstring, std::shared_ptr<DX12Wrapper::Texture>> m_sprites;
	};
}