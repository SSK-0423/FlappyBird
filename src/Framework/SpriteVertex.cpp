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
	// �ÓI�����o�ϐ��̎��̉�
	std::vector<SpriteVertex::Vertex> SpriteVertex::m_centerPivotVertex(0);
	std::vector<SpriteVertex::Vertex> SpriteVertex::m_topLeftPivotVertex(0);
	DX12Wrapper::VertexBuffer SpriteVertex::m_centerPivotVertexBuffer;
	DX12Wrapper::VertexBuffer SpriteVertex::m_topLeftPivotVertexBuffer;
	DX12Wrapper::IndexBuffer SpriteVertex::m_indexBuffer;

	Utility::RESULT SpriteVertex::Init()
	{
		// ���_�f�[�^�̐���
		// ���S���_�̒��_�f�[�^����
		m_centerPivotVertex = {
			{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0),DirectX::XMFLOAT2(0, 1) }, // ����
			{ DirectX::XMFLOAT3(-0.5f,  0.5f, 0),DirectX::XMFLOAT2(0, 0) }, // ����
			{ DirectX::XMFLOAT3(0.5f, -0.5f, 0),DirectX::XMFLOAT2(1, 1) }, // �E��
			{ DirectX::XMFLOAT3(0.5f,  0.5f, 0),DirectX::XMFLOAT2(1, 0) }, // �E��
		};
		// ���㌴�_�̒��_�f�[�^����
		m_topLeftPivotVertex = {
			{ DirectX::XMFLOAT3(0.0f, -1.0f, 0),DirectX::XMFLOAT2(0, 1) }, // ����
			{ DirectX::XMFLOAT3(0.0f,  0.0f, 0),DirectX::XMFLOAT2(0, 0) }, // ����
			{ DirectX::XMFLOAT3(1.0f, -1.0f, 0),DirectX::XMFLOAT2(1, 1) }, // �E��
			{ DirectX::XMFLOAT3(1.0f,  0.0f, 0),DirectX::XMFLOAT2(1, 0) }, // �E��
		};

		// ���_�o�b�t�@�̐���
		ID3D12Device& device = Dx12GraphicsEngine::Device();
		// ���S���_�̒��_�o�b�t�@�̐���
		if (m_centerPivotVertexBuffer.Create(
			device, &m_centerPivotVertex[0], SizeofVector<Vertex>(m_centerPivotVertex), sizeof(Vertex)) == RESULT::FAILED)
		{
			MessageBoxA(NULL, "VertexBuffer�̐����Ɏ��s", "�G���[", MB_OK);
			return RESULT::FAILED;
		}
		// ���㌴�_�̒��_�o�b�t�@�̐���
		if (m_topLeftPivotVertexBuffer.Create(
			device, &m_topLeftPivotVertex[0], SizeofVector<Vertex>(m_topLeftPivotVertex), sizeof(Vertex)) == RESULT::FAILED)
		{
			MessageBoxA(NULL, "VertexBuffer�̐����Ɏ��s", "�G���[", MB_OK);
			return RESULT::FAILED;
		}
		// �C���f�b�N�X�o�b�t�@�̐���
		if (m_indexBuffer.Create(device, { 0,1,2,2,1,3 }) == RESULT::FAILED)
		{
			MessageBoxA(NULL, "IndexBuffer�̐����Ɏ��s", "�G���[", MB_OK);
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