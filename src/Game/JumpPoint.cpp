#include "pch.h"
#include "JumpPoint.h"
#include "FlappyBird.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace DX12Wrapper;
using namespace Framework;

namespace FlappyBird
{
	// �ÓI�����o�ϐ��̎��̉�
	float JumpPoint::m_judgeLineX = 0.0f;
	float JumpPoint::m_currentPlayTime = 0.0f;
	UINT JumpPoint::m_jumpPointCount = 0;

	JumpPoint::JumpPoint(std::shared_ptr<Framework::Object> owner) : IComponent(owner)
	{
		m_owner.lock()->SetName("JumpPoint " + std::to_string(m_jumpPointCount));
		m_owner.lock()->SetTag("JumpPoint");
		m_jumpPointCount++;

		Sprite* sprite = new Sprite(L"res/texture/jump_point.png");
		SpriteRenderer* spriteRenderer = m_owner.lock()->AddComponent<SpriteRenderer>(m_owner.lock());
		spriteRenderer->SetSprite(sprite);
		spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GAMEOBJECT);
		spriteRenderer->SetLayer(static_cast<UINT>(SPRITE_LAYER::JUMP_POINT));

		auto& viewportSize = Dx12GraphicsEngine::GetViewport();

		m_transform = m_owner.lock()->GetComponent<Framework::Transform2D>();
		m_transform->position = { viewportSize.Width + 48.f, viewportSize.Height / 2.f };
		m_transform->scale = { 48.f, 48.f };
	}
	JumpPoint::~JumpPoint()
	{
	}
	void JumpPoint::Update(float deltaTime)
	{
		UpdatePosition();
	}
	void JumpPoint::Draw()
	{
	}
	void JumpPoint::DrawInspector()
	{
		if (ImGui::CollapsingHeader("JumpPoint"))
		{
			ImGui::Text("Timing: %f", m_timing);
		}
	}
	void JumpPoint::SetTiming(float timing)
	{
		m_timing = timing;
	}
	float JumpPoint::GetTiming()
	{
		return m_timing;
	}
	void JumpPoint::Reset()
	{
		m_owner.lock()->SetActive(false);
	}
	void JumpPoint::SetJudgeLineX(float judgeLineX)
	{
		m_judgeLineX = judgeLineX;
	}
	void JumpPoint::SetCurrentPlayTime(float currentPlayTime)
	{
		m_currentPlayTime = currentPlayTime;
	}
	void JumpPoint::UpdatePosition()
	{
		// �m�[�c�̃^�C�~���O�Ɣ��胉�C���̃^�C�~���O�̍����v�Z
		float diff = m_timing - m_currentPlayTime;

		auto& viewportSize = Dx12GraphicsEngine::GetViewport();

		// ��ʂ̉E�[���画�胉�C���܂ł̋���
		float distanceX = viewportSize.Width - m_judgeLineX;

		float x = (diff / 1000.f) * distanceX;

		// �m�[�c�̈ʒu���X�V
		m_transform->position.x = m_judgeLineX + x / 2.f;
	}
}