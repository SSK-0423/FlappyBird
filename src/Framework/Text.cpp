#include "pch.h"
#include "Text.h"
#include "Object.h"
#include "Transform2D.h"

using namespace DX12Wrapper;

namespace Framework
{
	Text::Text(Object* owner)
		: IComponent(owner)
	{
	}
	void Text::Update(float deltaTime)
	{
	}
	void Text::Draw()
	{
		auto transform = m_owner->GetComponent<Transform2D>();
		FontRenderer::DrawString(m_text, transform->position, transform->scale.x, m_color);
	}
	void Text::SetText(const std::wstring& text)
	{
		m_text = text;
	}
	void Text::SetColor(const DirectX::XMVECTORF32& color)
	{
		m_color = color;
	}
	void Text::SetPosition(const DirectX::XMFLOAT2& position)
	{
		m_owner->GetComponent<Transform2D>()->position = position;
	}
	void Text::SetScale(const float& scale)
	{
		m_owner->GetComponent<Transform2D>()->scale = { scale, scale };
	}
	void Text::SetRotation(float angle)
	{
		m_owner->GetComponent<Transform2D>()->angle = angle;
	}
}