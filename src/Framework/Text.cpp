#include "pch.h"
#include "Text.h"
#include "Object.h"
#include "Transform2D.h"

using namespace Utility;
using namespace DX12Wrapper;

namespace Framework
{
	Text::Text(std::shared_ptr<Object> owner)
		: IComponent(owner), m_text(L""), m_color(DirectX::Colors::Black)
	{
	}
	Text::~Text()
	{
		OutputDebugStringA("Text::~Text()\n");
	}
	void Text::Update(float deltaTime)
	{
	}
	void Text::Draw()
	{
		auto transform = m_owner.lock()->GetComponent<Transform2D>();
		FontRenderer::DrawString(m_text, transform->position, transform->scale.x, m_color);
	}
	void Text::DrawInspector()
	{
		char buffer[256];

		if (ImGui::CollapsingHeader("Text"))
		{
			wcharToChar(m_text.c_str(), buffer, _countof(buffer));
			ImGui::Text(buffer);
		}
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
		m_owner.lock()->GetComponent<Transform2D>()->position = position;
	}
	void Text::SetScale(const float& scale)
	{
		m_owner.lock()->GetComponent<Transform2D>()->scale = { scale, scale };
	}
	void Text::SetRotation(float angle)
	{
		m_owner.lock()->GetComponent<Transform2D>()->angle = angle;
	}
}