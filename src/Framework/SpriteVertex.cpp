#include "pch.h"
#include "SpriteVertex.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"
#include "DX12Wrapper/VertexBuffer.h"
#include "DX12Wrapper/IndexBuffer.h"

#include "Utility/EngineUtility.h"

using namespace DX12Wrapper;
using namespace Utility;

namespace Framework
{
	// 静的メンバ変数の実体化
	std::vector<SpriteVertex::Vertex> SpriteVertex::m_centerPivotVertex(0);
	std::vector<SpriteVertex::Vertex> SpriteVertex::m_topLeftPivotVertex(0);
	DX12Wrapper::VertexBuffer SpriteVertex::m_centerPivotVertexBuffer;
	DX12Wrapper::VertexBuffer SpriteVertex::m_topLeftPivotVertexBuffer;
	DX12Wrapper::IndexBuffer SpriteVertex::m_indexBuffer;

	Utility::RESULT SpriteVertex::Init()
	{
		// 頂点データの生成
		// 中心原点の頂点データ生成
		m_centerPivotVertex = {
			{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0),DirectX::XMFLOAT2(0, 1) }, // 左下
			{ DirectX::XMFLOAT3(-0.5f,  0.5f, 0),DirectX::XMFLOAT2(0, 0) }, // 左上
			{ DirectX::XMFLOAT3(0.5f, -0.5f, 0),DirectX::XMFLOAT2(1, 1) }, // 右下
			{ DirectX::XMFLOAT3(0.5f,  0.5f, 0),DirectX::XMFLOAT2(1, 0) }, // 右上
		};
		// 左上原点の頂点データ生成
		m_topLeftPivotVertex = {
			{ DirectX::XMFLOAT3(0.0f, -1.0f, 0),DirectX::XMFLOAT2(0, 1) }, // 左下
			{ DirectX::XMFLOAT3(0.0f,  0.0f, 0),DirectX::XMFLOAT2(0, 0) }, // 左上
			{ DirectX::XMFLOAT3(1.0f, -1.0f, 0),DirectX::XMFLOAT2(1, 1) }, // 右下
			{ DirectX::XMFLOAT3(1.0f,  0.0f, 0),DirectX::XMFLOAT2(1, 0) }, // 右上
		};

		// 頂点バッファの生成
		ID3D12Device& device = Dx12GraphicsEngine::Device();
		// 中心原点の頂点バッファの生成
		if (m_centerPivotVertexBuffer.Create(
			device, &m_centerPivotVertex[0], SizeofVector<Vertex>(m_centerPivotVertex), sizeof(Vertex)) == RESULT::FAILED)
		{
			MessageBoxA(NULL, "VertexBufferの生成に失敗", "エラー", MB_OK);
			return RESULT::FAILED;
		}
		// 左上原点の頂点バッファの生成
		if (m_topLeftPivotVertexBuffer.Create(
			device, &m_topLeftPivotVertex[0], SizeofVector<Vertex>(m_topLeftPivotVertex), sizeof(Vertex)) == RESULT::FAILED)
		{
			MessageBoxA(NULL, "VertexBufferの生成に失敗", "エラー", MB_OK);
			return RESULT::FAILED;
		}
		// インデックスバッファの生成
		if (m_indexBuffer.Create(device, { 0,1,2,2,1,3 }) == RESULT::FAILED)
		{
			MessageBoxA(NULL, "IndexBufferの生成に失敗", "エラー", MB_OK);
			return RESULT::FAILED;
		}

		return RESULT::SUCCESS;
	}

	DX12Wrapper::VertexBuffer& SpriteVertex::CenterPivotVertexBuffer()
	{
		return m_centerPivotVertexBuffer;
	}
	DX12Wrapper::VertexBuffer& SpriteVertex::TopLeftPivotVertexBuffer()
	{
		return m_topLeftPivotVertexBuffer;
	}
	DX12Wrapper::IndexBuffer& SpriteVertex::IndexBuffer()
	{
		return m_indexBuffer;
	}
}