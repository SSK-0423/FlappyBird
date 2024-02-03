#include "pch.h"
#include "BarLine.h"
#include "FlappyBird.h"

#include "imgui.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace DX12Wrapper;
using namespace Framework;

namespace FlappyBird
{
	// �ÓI�����o�ϐ��̏�����
	float BarLine::m_currentPlayTime = 0.0f;
	float BarLine::m_judgeLineX = 0.0f;

	BarLine::BarLine(Framework::Object* owner) : IComponent(owner)
	{
		// �X�v���C�g�ǉ�
		std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(L"res/texture/bar_line.png");
		SpriteRenderer* spriteRenderer = owner->AddComponent<SpriteRenderer>(m_owner);
		spriteRenderer->SetSprite(sprite);
		spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GUI);
		spriteRenderer->SetLayer(SPRITE_LAYER::UI);

		// �e�X�g�p��Transform2D
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
		if (m_timing <= 0.0f)
		{
			return;
		}

		if (m_transform->position.x < 0.f)
		{
			m_owner->SetActive(false);
			m_canPlaySE = true;

			// �ʒu������
			m_transform->position.x = Dx12GraphicsEngine::GetViewport().Width;
		}

		// �m�[�c�̃^�C�~���O�Ɣ��胉�C���̃^�C�~���O�̍����v�Z
		float diff = m_timing - m_currentPlayTime;

		auto viewportSize = Dx12GraphicsEngine::GetViewport();

		// ��ʂ̉E�[���画�胉�C���܂ł̋���
		float distanceX = viewportSize.Width - m_judgeLineX;

		float x = (diff / 1000.f) * distanceX;

		m_transform->position.x = m_judgeLineX + x / 2.f;
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
	bool BarLine::CanPlaySE()
	{
		return m_canPlaySE;
	}
	void BarLine::SetCanPlaySE(bool canPlaySE)
	{
		m_canPlaySE = canPlaySE;
	}
}