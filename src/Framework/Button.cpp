#include "pch.h"
#include "Button.h"
#include "Object.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "Text.h"
#include "Transform2D.h"
#include "InputSystem.h"

namespace Framework
{
	Button::Button(Object* owner) : IComponent(owner)
	{
		m_spriteRenderer = std::make_unique<SpriteRenderer>(m_owner);
	}
	bool Button::CheckClick()
	{
		auto& inputSystem = InputSystem::Instance();
		POINT mousePos = inputSystem.GetMousePosition();

		Transform2D* transform = m_owner->GetComponent<Transform2D>();
		float left = transform->position.x - transform->scale.x / 2;
		float right = transform->position.x + transform->scale.x / 2;
		float top = transform->position.y - transform->scale.y / 2;
		float bottom = transform->position.y + transform->scale.y / 2;

		// �}�E�X���{�^���͈͓̔��ɂ��邩
		if (left <= mousePos.x && mousePos.x <= right &&
			top <= mousePos.y && mousePos.y <= bottom)
		{
			// �}�E�X�N���b�N
			auto& inputSystem = InputSystem::Instance();
			if (inputSystem.GetMouseButtonDown(MOUSECODE::LEFT))
			{
				return true;
			}
		}
		return false;
	}
	void Button::Update(float deltaTime)
	{
		if (CheckClick())
		{
			m_onClick();
		}
	}
	void Button::Draw()
	{
		if (m_spriteRenderer != nullptr)
			m_spriteRenderer->Draw();
		if (m_text != nullptr)
			m_text->Draw();
	}
	void Button::SetPosition(float x, float y)
	{
		m_owner->GetComponent<Transform2D>()->position = { x, y };
	}
	void Button::SetScale(float x, float y)
	{
		m_owner->GetComponent<Transform2D>()->scale = { x, y };
	}
	void Button::SetTexture(const std::wstring& path)
	{
		m_spriteRenderer->SetSprite(new Sprite(path));
	}
	void Button::SetText(const std::wstring& text)
	{
		if (m_text == nullptr)
		{
			m_text = std::make_unique<Text>(m_owner);
		}
		m_text->SetText(text);
	}
	void Button::SetOnClick(const std::function<void()>& onClick)
	{
		m_onClick = onClick;
	}
}
