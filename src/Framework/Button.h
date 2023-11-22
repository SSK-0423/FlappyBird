#pragma once
#include "IComponent.h"
#include <DirectXMath.h>

namespace Framework
{
	class Button : public IComponent
	{
	public:
		Button(Object* owner);
		~Button() = default;

	private:
		std::unique_ptr<class SpriteRenderer> m_spriteRenderer = nullptr;
		std::unique_ptr<class Text> m_text = nullptr;
		std::function<void()> m_onClick = nullptr;

		bool CheckClick();
	public:
		void Update(float deltaTime) override;
		void Draw() override;
		void SetPosition(float x, float y);
		void SetScale(float x, float y);
		void SetTexture(const std::wstring& path);
		void SetText(const std::wstring& text);
		void SetOnClick(const std::function<void()>& onClick);
	};
}