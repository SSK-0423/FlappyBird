#include "Sprite.h"
#include "Utility/EngineUtility.h"

#include "DX12Wrapper/Texture.h"
#include "DX12Wrapper/VertexBuffer.h"
#include "DX12Wrapper/IndexBuffer.h"
#include "DX12Wrapper/DescriptorHeapCBV_SRV_UAV.h"
#include "DX12Wrapper/ShaderResourceViewDesc.h"

#include "DX12Wrapper/RenderingContext.h"

#include "Framework/Object.h"
#include "Framework/Transform2D.h"
#include "Framework/TextureManager.h"

using namespace Utility;
using namespace DX12Wrapper;

namespace Framework
{
	Sprite::Sprite(const std::wstring& path, SPRITE_PIVOT pivot)
	{
		m_pivot = pivot;

		m_descriptorHeap = std::make_unique<DescriptorHeapCBV_SRV_UAV>();

		ID3D12Device& device = Dx12GraphicsEngine::Device();
		if (m_descriptorHeap->Create(device) == RESULT::FAILED)
		{
			MessageBoxA(NULL, "DescriptorHeap‚Ì¶¬‚ÉŽ¸”s", "ƒGƒ‰[", MB_OK);
		}
		if (!path.empty())
		{
			LoadTexture(path);
		}
	}

	Sprite::~Sprite()
	{
	}

	void Sprite::LoadTexture(const std::wstring& path)
	{
		m_texture = TextureManager::LoadTexture(path);

		ID3D12Device& device = Dx12GraphicsEngine::Device();
		ShaderResourceViewDesc desc(*m_texture);
		m_descriptorHeap->RegistShaderResource(device, *m_texture, desc, 0);
	}
	void Sprite::SetTexture(std::shared_ptr<DX12Wrapper::Texture>& texture)
	{
		m_texture = texture;

		ID3D12Device& device = Dx12GraphicsEngine::Device();
		ShaderResourceViewDesc desc(*m_texture);
		m_descriptorHeap->RegistShaderResource(device, *m_texture, desc, 0);
	}
	Texture& Sprite::GetTexture() const
	{
		return *m_texture.get();
	}
	DescriptorHeapCBV_SRV_UAV& Sprite::GetDescriptorHeap() const
	{
		return *m_descriptorHeap.get();
	}
	SPRITE_PIVOT Sprite::GetPivot() const
	{
		return m_pivot;
	}
}