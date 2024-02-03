#include "pch.h"
#include "BarLine.h"
#include "FlappyBird.h"

#include "imgui.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace DX12Wrapper;
using namespace Framework;

namespace FlappyBird
{
	BarLine::BarLine(Framework::Object* owner) : IComponent(owner)
	{
		// スプライト追加
		std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(L"res/texture/bar_line.png");
		SpriteRenderer* spriteRenderer = owner->AddComponent<SpriteRenderer>(m_owner);
		spriteRenderer->SetSprite(sprite);
		spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GUI);
		spriteRenderer->SetLayer(SPRITE_LAYER::UI);
		//spriteRenderer->SetActive(false);

		// テスト用のTransform2D
		auto viewportSize = Dx12GraphicsEngine::GetViewport();
		Transform2D* transform = owner->GetComponent<Transform2D>();
		transform->position = { viewportSize.Width, viewportSize.Height / 2.f };
		transform->scale = { 5.f, viewportSize.Height };
	}
	BarLine::~BarLine()
	{
	}
	void BarLine::Update(float deltaTime)
	{
		if (m_timing <= 0.0f)
		{
			return;
		}

		// 自分のタイミングでノーツを生成する

		// 現在の再生時間を取得

		// ノーツ判定ライン到達タイミングと比較

		// 到達タイミングになったらノーツを生成

	}
	void BarLine::Draw()
	{
	}
	void BarLine::DrawInspector()
	{
		ImGui::SeparatorText("BarLine");
		ImGui::Text("Timing: %f", m_timing);
	}
	void BarLine::SetTiming(float timing)
	{
		m_timing = timing;
	}
}