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
			MessageBoxA(NULL, "RootSignatureの生成に失敗", "エラー", MB_OK);
		}
		if (CreateGraphicsPipelineState(device) == RESULT::FAILED)
		{
			MessageBoxA(NULL, "GraphicsPipelineStateの生成に失敗", "エラー", MB_OK);
		}
		if (m_drawModeBuffer->Create(device, &m_drawMode, sizeof(SPRITE_DRAW_MODE)) == RESULT::FAILED)
		{
			MessageBoxA(NULL, "ContantBufferの生成に失敗", "エラー", MB_OK);
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
		// エディター上でスプライトを表示するために登録
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
		// エディター上でスプライトを表示するために登録
		Editor::SetTexture(m_sprites.back()->GetTexture());
#endif // _DEBUG
	}
	void SpriteRenderer::AddSprite(Sprite* sprite)
	{
		m_sprites.push_back(std::shared_ptr<Sprite>(sprite));

#ifdef _DEBUG
		// エディター上でスプライトを表示するために登録
		Editor::SetTexture(m_sprites.back()->GetTexture());
#endif // _DEBUG
	}
	void SpriteRenderer::AddSprite(std::shared_ptr<class Sprite> sprite)
	{
		m_sprites.push_back(sprite);

#ifdef _DEBUG
		// エディター上でスプライトを表示するために登録
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
		// レイヤーが小さい程手前に描画される
		m_owner.lock()->GetComponent<Transform2D>()->depth = static_cast<float>(layer) / SPRITE_LAYER_MAX;
	}
	void SpriteRenderer::Start()
	{
		ID3D12Device& device = Dx12GraphicsEngine::Device();

		// マテリアルがない場合はマテリアルを生成
		Material* material = m_owner.lock()->GetComponent<Material>();

		// マテリアルがない場合はマテリアルを生成
		if (material == nullptr)
		{
			material = m_owner.lock()->AddComponent<Material>(m_owner.lock());
		}

		// 全スプライトのディスクリプタヒープにレンダリングに必要なデータを登録
		for (auto sprite : m_sprites)
		{
			// モデル行列をセット
			sprite->GetDescriptorHeap().RegistConstantBuffer(
				device,
				m_owner.lock()->GetComponent<Transform2D>()->GetConstantBuffer(),
				static_cast<UINT>(CONSTANT_BUFFER_INDEX::TRANSFORM));

			// ビュープロジェクション行列をセット
			sprite->GetDescriptorHeap().RegistConstantBuffer(
				device,
				Scene::GetCamera().GetConstantBuffer(),
				static_cast<UINT>(CONSTANT_BUFFER_INDEX::CAMERA));

			// 描画モードをセット
			sprite->GetDescriptorHeap().RegistConstantBuffer(
				device,
				*m_drawModeBuffer.get(),
				static_cast<UINT>(CONSTANT_BUFFER_INDEX::DRAW_MODE));

			// マテリアルをセット
			sprite->GetDescriptorHeap().RegistConstantBuffer(
				Dx12GraphicsEngine::Device(),
				material->GetConstantBuffer(),
				static_cast<UINT>(CONSTANT_BUFFER_INDEX::MATERIAL));
		}

		// 0番目のスプライトをセット
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
		// 描画対象のスプライトがない場合は描画しない
		if (m_sprites.size() == 0)
		{
			return;
		}

		RenderingContext renderingContext = Dx12GraphicsEngine::GetRenderingContext();

		auto& renderSprite = m_sprites[m_currentSpriteIndex];

		// スプライトの描画コール発行
		renderingContext.SetGraphicsRootSignature(*m_rootSignature);
		renderingContext.SetPipelineState(*m_pipelineState);

		// ここは各スプライト固有のデータをセットする
		renderingContext.SetDescriptorHeap(renderSprite->GetDescriptorHeap());

		// ピボット毎に異なる
		renderingContext.SetVertexBuffer(0, renderSprite->GetVertexBuffer());

		// インデックスデータ、トポロジーは全て一緒
		renderingContext.SetIndexBuffer(renderSprite->GetIndexBuffer());
		renderingContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// ここでスプライト数分描画する
		renderingContext.DrawIndexedInstanced(renderSprite->GetIndexBuffer().GetIndexNum(), 1);
	}

	void SpriteRenderer::DrawInspector()
	{
		if (ImGui::CollapsingHeader("SpriteRenderer"))
		{
			// 描画レイヤーの表示
			ImGui::Text("Draw Layer: %u", m_layer);

			// セットされているスプライトの表示
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
		// ルートシグネチャとシェーダーセット
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineState = {};
		pipelineState.pRootSignature = &m_rootSignature->GetRootSignature();
		pipelineState.VS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::GetShader("SpriteVS")->GetShader());
		pipelineState.PS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::GetShader("SpritePS")->GetShader());

		// サンプルマスク設定
		pipelineState.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		// ブレンド
		pipelineState.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		pipelineState.BlendState.AlphaToCoverageEnable = true;
		pipelineState.BlendState.RenderTarget[0].BlendEnable = true;
		pipelineState.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		pipelineState.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

		// ラスタライズ設定
		pipelineState.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineState.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;

		// インプットレイアウトの設定
		InputLayout layout;
		layout.Append("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0);
		layout.Append("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0);
		pipelineState.InputLayout.pInputElementDescs = layout.GetData();
		pipelineState.InputLayout.NumElements = layout.GetSize();
		pipelineState.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		pipelineState.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		// デプスステンシル設定
		pipelineState.DepthStencilState.DepthEnable = true;
		pipelineState.DepthStencilState.StencilEnable = false;
		pipelineState.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		pipelineState.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		pipelineState.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		// レンダーターゲットの設定
		pipelineState.NumRenderTargets = 1;
		pipelineState.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;	// カラー

		// アンチエイリアシングのためのサンプル数設定
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
