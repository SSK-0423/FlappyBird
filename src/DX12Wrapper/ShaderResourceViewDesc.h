#pragma once

#include <d3d12.h>

namespace DX12Wrapper
{
	class Texture;

	struct ShaderResourceViewDesc {
		D3D12_SHADER_RESOURCE_VIEW_DESC desc;
		ShaderResourceViewDesc(Texture& texture, bool isCubeTex = false);
		~ShaderResourceViewDesc();
	};
}