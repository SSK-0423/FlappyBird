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

using namespace Utility;
using namespace DX12Wrapper;

namespace Framework
{
	Sprite::Sprite(const std::wstring& path, SPRITE_PIVOT pivot)
	{
		if (pivot == SPRITE_PIVOT::CENTER)
		{
			m_vertex.push_back({ DirectX::XMFLOAT3(-0.5f, -0.5f, 0),DirectX::XMFLOAT2(0, 1) }); // 左下
			m_vertex.push_back({ DirectX::XMFLOAT3(-0.5f,  0.5f, 0),DirectX::XMFLOAT2(0, 0) }); // 左上
			m_vertex.push_back({ DirectX::XMFLOAT3(0.5f, -0.5f, 0),DirectX::XMFLOAT2(1, 1) }); // 右下
			m_vertex.push_back({ DirectX::XMFLOAT3(0.5f,  0.5f, 0),DirectX::XMFLOAT2(1, 0) }); // 右上
		}
		else if (pivot == SPRITE_PIVOT::TOP_LEFT)
		{
			m_vertex.push_back({ DirectX::XMFLOAT3(0.0f, -1.0f, 0),DirectX::XMFLOAT2(0, 1) }); // 左下
			m_vertex.push_back({ DirectX::XMFLOAT3(0.0f,  0.0f, 0),DirectX::XMFLOAT2(0, 0) }); // 左上
			m_vertex.push_back({ DirectX::XMFLOAT3(1.0f, -1.0f, 0),DirectX::XMFLOAT2(1, 1) }); // 右下
			m_vertex.push_back({ DirectX::XMFLOAT3(1.0f,  0.0f, 0),DirectX::XMFLOAT2(1, 0) }); // 右上
		}

		m_vertexBuffer = std::make_unique<VertexBuffer>();
		m_indexBuffer = std::make_unique<IndexBuffer>();
		m_descriptorHeap = std::make_unique<DescriptorHeapCBV_SRV_UAV>();

		ID3D12Device& device = Dx12GraphicsEngine::Instance().Device();
		if (m_vertexBuffer->Create(
			device, &m_vertex[0], SizeofVector<SpriteVertex>(m_vertex), sizeof(SpriteVertex)) == RESULT::FAILED)
		{
			MessageBoxA(NULL, "VertexBufferの生成に失敗", "エラー", MB_OK);
		}
		if (m_indexBuffer->Create(device, { 0,1,2,2,1,3 }) == RESULT::FAILED)
		{
			MessageBoxA(NULL, "IndexBufferの生成に失敗", "エラー", MB_OK);
		}
		if (m_descriptorHeap->Create(device) == RESULT::FAILED)
		{
			MessageBoxA(NULL, "DescriptorHeapの生成に失敗", "エラー", MB_OK);
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
		if (m_texture == nullptr)
		{
			m_texture = std::make_shared<Texture>();
		}

		RESULT result = m_texture->CreateTextureFromWIC(Dx12GraphicsEngine::Instance(), path);
		if (result == RESULT::FAILED)
		{
			MessageBoxA(NULL, "テクスチャ読み込みに失敗しました。", "エラー", MB_OK);
		}

		ID3D12Device& device = Dx12GraphicsEngine::Instance().Device();
		ShaderResourceViewDesc desc(*m_texture);
		m_descriptorHeap->RegistShaderResource(device, *m_texture, desc, 0);
	}
	void Sprite::SetTexture(std::shared_ptr<DX12Wrapper::Texture>& texture)
	{
		m_texture = texture;

		ID3D12Device& device = Dx12GraphicsEngine::Instance().Device();
		ShaderResourceViewDesc desc(*m_texture);
		m_descriptorHeap->RegistShaderResource(device, *m_texture, desc, 0);

	}
	DescriptorHeapCBV_SRV_UAV& Sprite::GetDescriptorHeap() const
	{
		return *m_descriptorHeap.get();
	}
	VertexBuffer& Sprite::GetVertexBuffer() const
	{
		return *m_vertexBuffer.get();
	}
	DX12Wrapper::IndexBuffer& Sprite::GetIndexBuffer() const
	{
		return *m_indexBuffer.get();
	}
}