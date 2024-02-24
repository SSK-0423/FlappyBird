#pragma once

#include <DirectXMath.h>

#include "Utility/EngineUtility.h"

namespace DX12Wrapper
{
	class VertexBuffer;
	class IndexBuffer;
}

namespace Framework
{
	/// <summary>
	/// プリミティブデータを生成するクラス
	/// </summary>
	class SpriteVertex
	{
	public:
		static Utility::RESULT Init();
		static DX12Wrapper::VertexBuffer& CenterPivotVertexBuffer();
		static DX12Wrapper::VertexBuffer& TopLeftPivotVertexBuffer();
		static DX12Wrapper::IndexBuffer& IndexBuffer();

	private:
		SpriteVertex() = default;
		~SpriteVertex() = default;

		struct Vertex
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT2 uv;
		};
		static std::vector<Vertex> m_centerPivotVertex;		         // 中心原点の頂点データ
		static std::vector<Vertex> m_topLeftPivotVertex;	         // 左上原点の頂点データ

		static DX12Wrapper::VertexBuffer m_centerPivotVertexBuffer;  // 中心原点の頂点バッファ
		static DX12Wrapper::VertexBuffer m_topLeftPivotVertexBuffer; // 左上原点の頂点バッファ
		static DX12Wrapper::IndexBuffer m_indexBuffer;               // インデックスバッファ
	};
}