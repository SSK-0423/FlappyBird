#include "pch.h"
#include "SpriteRenderer.h"
#include "Object.h"
#include "Sprite.h"
#include "ShaderLibrary.h"
#include "Transform2D.h"
#include "Material.h"
#include "Camera.h"
#include "Scene.h"
#include "Editor.h"
#include "imgui.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"
#include "DX12Wrapper/VertexBuffer.h"
#include "DX12Wrapper/IndexBuffer.h"
#include "DX12Wrapper/ConstantBuffer.h"
#include "DX12Wrapper/GraphicsPipelineState.h"
#include "DX12Wrapper/RootSignature.h"
#include "DX12Wrapper/DescriptorHeapCBV_SRV_UAV.h"
#include "DX12Wrapper/RenderingContext.h"
#include "DX12Wrapper/InputLayout.h"

using namespace Utility;
using namespace DX12Wrapper;

constexpr int SPRITE_LAYER_MAX = 32;

namespace Framework
{
	SpriteRenderer::SpriteRenderer(std::shared_ptr<Object> owner)
		: IComponent(owner)
	{
		m_rootSignature = std::make_shared<RootSignature>();
		m_pipelineState = std::make_shared<GraphicsPipelineState>();
		m_drawModeBuffer = std::make_shared<ConstantBuffer>();

		ID3D12Device& device = Dx12GraphicsEngine::Device();
		if (CreateRootSignature(device) == RESULT::FAILED)
		{
			MessageBoxA(NULL, "RootSignature�̐����Ɏ��s", "�G���[", MB_OK);
		}
		if (CreateGraphicsPipelineState(device) == RESULT::FAILED)
		{
			MessageBoxA(NULL, "GraphicsPipelineState�̐����Ɏ��s", "�G���[", MB_OK);
		}
		if (m_drawModeBuffer->Create(device, &m_drawMode, sizeof(SPRITE_DRAW_MODE)) == RESULT::FAILED)
		{
			MessageBoxA(NULL, "ContantBuffer�̐����Ɏ��s", "�G���[", MB_OK);
		}
	}
	SpriteRenderer::~SpriteRenderer()
	{
	}
	void SpriteRenderer::SetSprite(Sprite* sprite)
	{
		if (m_sprites.size() > 0)
		{
			m_sprites[0] = std::shared_ptr<Sprite>(sprite);
		}
		else
		{
			m_sprites.push_back(std::shared_ptr<Sprite>(sprite));
		}

#ifdef _DEBUG
		// �G�f�B�^�[��ŃX�v���C�g��\�����邽�߂ɓo�^
		Editor::SetTexture(m_sprites.back()->GetTexture());
#endif // _DEBUG
	}
	void SpriteRenderer::SetSprite(std::shared_ptr<Sprite> sprite)
	{
		if (m_sprites.size() > 0)
		{
			m_sprites[0] = sprite;
		}
		else
		{
			m_sprites.push_back(sprite);
		}

#ifdef _DEBUG
		// �G�f�B�^�[��ŃX�v���C�g��\�����邽�߂ɓo�^
		Editor::SetTexture(m_sprites.back()->GetTexture());
#endif // _DEBUG
	}
	void SpriteRenderer::AddSprite(Sprite* sprite)
	{
		m_sprites.push_back(std::shared_ptr<Sprite>(sprite));

#ifdef _DEBUG
		// �G�f�B�^�[��ŃX�v���C�g��\�����邽�߂ɓo�^
		Editor::SetTexture(m_sprites.back()->GetTexture());
#endif // _DEBUG
	}
	void SpriteRenderer::AddSprite(std::shared_ptr<class Sprite> sprite)
	{
		m_sprites.push_back(sprite);

#ifdef _DEBUG
		// �G�f�B�^�[��ŃX�v���C�g��\�����邽�߂ɓo�^
		Editor::SetTexture(m_sprites.back()->GetTexture());
#endif // _DEBUG
	}
	void SpriteRenderer::ChangeRenderSprite(size_t index)
	{
		assert(index < m_sprites.size());

		m_currentSpriteIndex = index;
	}
	void SpriteRenderer::SetDrawMode(SPRITE_DRAW_MODE drawMode)
	{
		m_drawMode = drawMode;
		m_drawModeBuffer->UpdateData(&m_drawMode);
	}
	void SpriteRenderer::SetLayer(UINT layer)
	{
		m_layer = layer;
		// ���C���[������������O�ɕ`�悳���
		m_owner.lock()->GetComponent<Transform2D>()->depth = static_cast<float>(layer) / SPRITE_LAYER_MAX;
	}
	void SpriteRenderer::Start()
	{
		ID3D12Device& device = Dx12GraphicsEngine::Device();

		// �}�e���A�����Ȃ��ꍇ�̓}�e���A���𐶐�
		Material* material = m_owner.lock()->GetComponent<Material>();

		// �}�e���A�����Ȃ��ꍇ�̓}�e���A���𐶐�
		if (material == nullptr)
		{
			material = m_owner.lock()->AddComponent<Material>(m_owner.lock());
		}

		// �S�X�v���C�g�̃f�B�X�N���v�^�q�[�v�Ƀ����_�����O�ɕK�v�ȃf�[�^��o�^
		for (auto sprite : m_sprites)
		{
			// ���f���s����Z�b�g
			sprite->GetDescriptorHeap().RegistConstantBuffer(
				device,
				m_owner.lock()->GetComponent<Transform2D>()->GetConstantBuffer(),
				static_cast<UINT>(CONSTANT_BUFFER_INDEX::TRANSFORM));

			// �r���[�v���W�F�N�V�����s����Z�b�g
			sprite->GetDescriptorHeap().RegistConstantBuffer(
				device,
				Scene::GetCamera().GetConstantBuffer(),
				static_cast<UINT>(CONSTANT_BUFFER_INDEX::CAMERA));

			// �`�惂�[�h���Z�b�g
			sprite->GetDescriptorHeap().RegistConstantBuffer(
				device,
				*m_drawModeBuffer.get(),
				static_cast<UINT>(CONSTANT_BUFFER_INDEX::DRAW_MODE));

			// �}�e���A�����Z�b�g
			sprite->GetDescriptorHeap().RegistConstantBuffer(
				Dx12GraphicsEngine::Device(),
				material->GetConstantBuffer(),
				static_cast<UINT>(CONSTANT_BUFFER_INDEX::MATERIAL));
		}

		// 0�Ԗڂ̃X�v���C�g���Z�b�g
		if (m_sprites.size() > 0)
		{
			m_currentSpriteIndex = 0;
		}
	}
	void SpriteRenderer::Update(float deltaTime)
	{
	}
	void SpriteRenderer::Draw()
	{
		// �`��Ώۂ̃X�v���C�g���Ȃ��ꍇ�͕`�悵�Ȃ�
		if (m_sprites.size() == 0)
		{
			return;
		}

		RenderingContext renderingContext = Dx12GraphicsEngine::GetRenderingContext();

		auto& renderSprite = m_sprites[m_currentSpriteIndex];

		// �X�v���C�g�̕`��R�[�����s
		renderingContext.SetGraphicsRootSignature(*m_rootSignature);
		renderingContext.SetPipelineState(*m_pipelineState);

		// �����͊e�X�v���C�g�ŗL�̃f�[�^���Z�b�g����
		renderingContext.SetDescriptorHeap(renderSprite->GetDescriptorHeap());

		// �s�{�b�g���ɈقȂ�
		renderingContext.SetVertexBuffer(0, renderSprite->GetVertexBuffer());

		// �C���f�b�N�X�f�[�^�A�g�|���W�[�͑S�Ĉꏏ
		renderingContext.SetIndexBuffer(renderSprite->GetIndexBuffer());
		renderingContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// �����ŃX�v���C�g�����`�悷��
		renderingContext.DrawIndexedInstanced(renderSprite->GetIndexBuffer().GetIndexNum(), 1);
	}

	void SpriteRenderer::DrawInspector()
	{
		if (ImGui::CollapsingHeader("SpriteRenderer"))
		{
			// �`�惌�C���[�̕\��
			ImGui::Text("Draw Layer: %u", m_layer);

			// �Z�b�g����Ă���X�v���C�g�̕\��
			ImGui::Text("Sprites");
			for (size_t i = 0; i < m_sprites.size(); i++)
			{
				ImGui::Text("Sprite Index: %d", i);
				ImGui::Image(Editor::GetTextureID(m_sprites[i]->GetTexture()), ImVec2(100, 100));
			}
		}
	}

	Utility::RESULT SpriteRenderer::CreateGraphicsPipelineState(ID3D12Device& device)
	{
		// ���[�g�V�O�l�`���ƃV�F�[�_�[�Z�b�g
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineState = {};
		pipelineState.pRootSignature = &m_rootSignature->GetRootSignature();
		pipelineState.VS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::GetShader("SpriteVS")->GetShader());
		pipelineState.PS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::GetShader("SpritePS")->GetShader());

		// �T���v���}�X�N�ݒ�
		pipelineState.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		// �u�����h
		pipelineState.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		pipelineState.BlendState.AlphaToCoverageEnable = true;
		pipelineState.BlendState.RenderTarget[0].BlendEnable = true;
		pipelineState.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		pipelineState.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

		// ���X�^���C�Y�ݒ�
		pipelineState.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineState.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;

		// �C���v�b�g���C�A�E�g�̐ݒ�
		InputLayout layout;
		layout.Append("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0);
		layout.Append("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0);
		pipelineState.InputLayout.pInputElementDescs = layout.GetData();
		pipelineState.InputLayout.NumElements = layout.GetSize();
		pipelineState.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		pipelineState.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		// �f�v�X�X�e���V���ݒ�
		pipelineState.DepthStencilState.DepthEnable = true;
		pipelineState.DepthStencilState.StencilEnable = false;
		pipelineState.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		pipelineState.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		pipelineState.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		// �����_�[�^�[�Q�b�g�̐ݒ�
		pipelineState.NumRenderTargets = 1;
		pipelineState.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;	// �J���[

		// �A���`�G�C���A�V���O�̂��߂̃T���v�����ݒ�
		pipelineState.SampleDesc.Count = 1;
		pipelineState.SampleDesc.Quality = 0;

		return m_pipelineState->Create(device, pipelineState);
	}

	Utility::RESULT SpriteRenderer::CreateRootSignature(ID3D12Device& device)
	{
		RootSignatureData rootSigData;
		rootSigData.m_descRangeData.cbvDescriptorNum = static_cast<UINT>(CONSTANT_BUFFER_INDEX::BUFFER_COUNT);
		rootSigData.m_descRangeData.srvDescriptorNum = 1;

		return m_rootSignature->Create(device, rootSigData);
	}
}
