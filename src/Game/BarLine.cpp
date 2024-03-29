#include "pch.h"
#include "BarLine.h"
#include "FlappyBird.h"

#include "imgui.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace DX12Wrapper;
using namespace Framework;

namespace FlappyBird
{
	// 静的メンバ変数の初期化
	float BarLine::m_currentPlayTime = 0.0f;
	float BarLine::m_judgeLineX = 0.0f;

	BarLine::BarLine(std::shared_ptr<Framework::Object> owner) : IComponent(owner)
	{
		// テスト用のTransform2D
		auto viewportSize = Dx12GraphicsEngine::GetViewport();
		m_transform = owner->GetComponent<Transform2D>();
		m_transform->position = { viewportSize.Width, viewportSize.Height / 2.f };
		m_transform->scale = { 5.f, viewportSize.Height };
	}
	BarLine::~BarLine()
	{
	}
	void BarLine::Update(float deltaTime)
	{
		if (m_timing < 0.0f)
		{
			return;
		}

		if (m_transform->position.x < 0.f)
		{
			m_owner.lock()->SetActive(false);
		}

		UpdatePosition();
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
	float BarLine::GetTiming()
	{
		return m_timing;
	}
	void BarLine::SetCurrentPlayTime(float currentPlayTime)
	{
		m_currentPlayTime = currentPlayTime;
	}
	void BarLine::SetJudgeLineX(float judgeLineX)
	{
		m_judgeLineX = judgeLineX;
	}
	void BarLine::UpdatePosition()
	{
		// ノーツのタイミングと判定ラインのタイミングの差を計算
		float diff = m_timing - m_currentPlayTime;

		auto viewportSize = Dx12GraphicsEngine::GetViewport();

		// 画面の右端から判定ラインまでの距離
		float distanceX = viewportSize.Width - m_judgeLineX;

		float x = (diff / 1000.f) * distanceX;

		m_transform->position.x = m_judgeLineX + x / 2.f;
	}
	bool BarLine::CanPlaySE()
	{
		return m_canPlaySE;
	}
	void BarLine::SetCanPlaySE(bool canPlaySE)
	{
		m_canPlaySE = canPlaySE;
	}
	void BarLine::SetBarLineType(BARLINETYPE type)
	{
		// スプライト追加
		std::shared_ptr<Sprite> sprite;

		switch (type)
		{
		case BARLINETYPE::WHOLE:
			sprite = std::make_shared<Sprite>(L"res/texture/bar_line_whole.png");
			break;
		case BARLINETYPE::QUARTER:
			sprite = std::make_shared<Sprite>(L"res/texture/bar_line_quarter.png");
			break;
		case BARLINETYPE::EIGHTH:
			sprite = std::make_shared<Sprite>(L"res/texture/bar_line_eighth.png");
			break;
		}

		SpriteRenderer* spriteRenderer = m_owner.lock()->AddComponent<SpriteRenderer>(m_owner.lock());
		spriteRenderer->SetSprite(sprite);
		spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GUI);
		spriteRenderer->SetLayer(SPRITE_LAYER::BARLINE);
	}
}