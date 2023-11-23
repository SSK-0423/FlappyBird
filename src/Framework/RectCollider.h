#pragma once

#include "Collider.h"

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

		Rect(float x, float y, float width, float height)
			: x(x), y(y), width(width), height(height)
		{}
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