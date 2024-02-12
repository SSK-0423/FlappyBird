#pragma once
#include "IComponent.h"
#include "DX12Wrapper/FontRenderer.h"

#include <DirectXMath.h>

namespace Framework
{
	/// <summary>
	/// テキストを描画するコンポーネント
	/// </summary>
	class Text : public IComponent
	{
	public:
		Text(std::shared_ptr<Object> owner);
		~Text();

		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		void SetText(const std::wstring& text);
		void SetColor(const DirectX::XMVECTORF32& color);
		void SetPosition(const DirectX::XMFLOAT2& position);

		/// <summary>
		/// テキストの大きさを設定する
		/// ピクセル単位ではないので、1.0fを基準に大きさを設定すること
		/// </summary>
		/// <param name="scale">スケール</param>
		void SetScale(const float& scale);

		void SetRotation(float angle);

	private:
		std::wstring m_text;
		DirectX::XMVECTORF32 m_color;
	};
}