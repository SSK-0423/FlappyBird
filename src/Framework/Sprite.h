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
	enum class SPRITE_PIVOT
	{
		TOP_LEFT,
		CENTER,
	};

	class Sprite
	{
	public:
		Sprite(const std::wstring& path, SPRITE_PIVOT pivot = SPRITE_PIVOT::CENTER);
		~Sprite();

		void LoadTexture(const std::wstring& path);
		void SetTexture(std::shared_ptr<DX12Wrapper::Texture>& texture);

		D3D12_GPU_DESCRIPTOR_HANDLE GetTextureHandle() const;
		DX12Wrapper::Texture& GetTexture() const;
		DX12Wrapper::DescriptorHeapCBV_SRV_UAV& GetDescriptorHeap() const;
		DX12Wrapper::VertexBuffer& GetVertexBuffer() const;
		DX12Wrapper::IndexBuffer& GetIndexBuffer() const;

	private:
		struct SpriteVertex
		{
			DirectX::XMFLOAT3 vertex;
			DirectX::XMFLOAT2 uv;
		};
		std::vector<SpriteVertex> m_vertex;

		// 複数のオブジェクトで同じテクスチャを使う場合を考慮
		std::shared_ptr<DX12Wrapper::Texture> m_texture = nullptr;
		std::unique_ptr<DX12Wrapper::VertexBuffer> m_vertexBuffer = nullptr;
		std::unique_ptr<DX12Wrapper::IndexBuffer> m_indexBuffer = nullptr;
		std::unique_ptr<DX12Wrapper::DescriptorHeapCBV_SRV_UAV> m_descriptorHeap = nullptr;
	};
}