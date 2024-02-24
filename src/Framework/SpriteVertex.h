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
	/// �v���~�e�B�u�f�[�^�𐶐�����N���X
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
		static std::vector<Vertex> m_centerPivotVertex;		         // ���S���_�̒��_�f�[�^
		static std::vector<Vertex> m_topLeftPivotVertex;	         // ���㌴�_�̒��_�f�[�^

		static DX12Wrapper::VertexBuffer m_centerPivotVertexBuffer;  // ���S���_�̒��_�o�b�t�@
		static DX12Wrapper::VertexBuffer m_topLeftPivotVertexBuffer; // ���㌴�_�̒��_�o�b�t�@
		static DX12Wrapper::IndexBuffer m_indexBuffer;               // �C���f�b�N�X�o�b�t�@
	};
}