#pragma once
#include "IComponent.h"
#include "DX12Wrapper/FontRenderer.h"

#include <DirectXMath.h>

namespace Framework
{
	class Text : public IComponent
	{
	public:
		Text(Object* owner);
		~Text() = default;

		void Update(float deltaTime) override;
		void Draw() override;

		void SetText(const std::wstring& text);
		void SetColor(const DirectX::XMVECTORF32& color);
		void SetPosition(const DirectX::XMFLOAT2& position);
		void SetScale(const float& scale);
		void SetRotation(float angle);

	private:
		std::wstring m_text;
		DirectX::XMVECTORF32 m_color;
	};
}