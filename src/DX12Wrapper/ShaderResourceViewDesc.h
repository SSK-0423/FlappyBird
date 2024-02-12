#pragma once

#include <d3d12.h>

namespace DX12Wrapper
{
	class Texture;

	/// <summary>
	/// �V�F�[�_�[���\�[�X�r���[�𐶐����邽�߂̍\���̂̃��b�p�[
	/// </summary>
	struct ShaderResourceViewDesc {
		D3D12_SHADER_RESOURCE_VIEW_DESC desc;
		ShaderResourceViewDesc(Texture& texture, bool isCubeTex = false);
		~ShaderResourceViewDesc();
	};
}