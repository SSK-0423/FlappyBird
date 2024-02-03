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
		// �X�v���C�g�ǉ�
		std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(L"res/texture/bar_line.png");
		SpriteRenderer* spriteRenderer = owner->AddComponent<SpriteRenderer>(m_owner);
		spriteRenderer->SetSprite(sprite);
		spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GUI);
		spriteRenderer->SetLayer(SPRITE_LAYER::UI);
		//spriteRenderer->SetActive(false);

		// �e�X�g�p��Transform2D
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

		// �����̃^�C�~���O�Ńm�[�c�𐶐�����

		// ���݂̍Đ����Ԃ��擾

		// �m�[�c���胉�C�����B�^�C�~���O�Ɣ�r

		// ���B�^�C�~���O�ɂȂ�����m�[�c�𐶐�

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