#pragma once

#include "Collider.h"

#include <DirectXMath.h>

namespace Framework
{
	struct Rect
	{
		// ピクセル単位での座標
		float x;
		float y;

		// ピクセル単位での幅と高さ
		float width;
		float height;

		// 左上原点かどうか
		bool isLeftUpOrigin;

		Rect(float x, float y, float width, float height, bool isLeftUpOrigin)
			: x(x), y(y), width(width), height(height), isLeftUpOrigin(isLeftUpOrigin)
		{}

		DirectX::XMFLOAT2 GetCenter() const
		{
			// 左上が原点の場合
			if (isLeftUpOrigin)
				return DirectX::XMFLOAT2(x + width / 2, y - height / 2);
			// 中心が原点の場合
			else
				return DirectX::XMFLOAT2(x, y);
		}
	};

	class RectCollider : public Collider
	{
	public:
		RectCollider(Object* owner);
		~RectCollider();

		void Update(float deltaTime) override;
		void Draw() override;
		void* GetColliderShape() const override;

		void SetRectSize(float width, float height);

	private:
		Rect m_rect;
	};
}