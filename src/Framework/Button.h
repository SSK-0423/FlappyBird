#pragma once
#include "IComponent.h"
#include <DirectXMath.h>

namespace Framework
{
	class SpriteRenderer;
	class Transform2D;

	/// <summary>
	/// ボタンコンポーネント
	/// </summary>
	class Button : public IComponent
	{
	public:
		Button(std::shared_ptr<Object> owner);
		~Button() = default;

	private:
		SpriteRenderer* m_spriteRenderer = nullptr;
		std::unique_ptr<class Text> m_text = nullptr;
		std::function<void()> m_onClick = nullptr;	// クリック時のコールバック関数

		bool CheckClick();
	public:
		void Update(float deltaTime) override;
		void Draw() override;
		void SetPosition(const DirectX::XMFLOAT2& position);
		void SetScale(const DirectX::XMFLOAT2& scale);
		void SetTexture(const std::wstring& path);
		void SetText(const std::wstring& text);
		void SetLayer(int layer);
		
		/// <summary>
		/// ボタンがクリックされたときのコールバック関数を設定する
		/// </summary>
		/// <param name="onClick">コールバック関数</param>
		void SetOnClick(const std::function<void()>& onClick);
	};
}