#pragma once

#include "Collider.h"

namespace Framework
{
	struct Rect
	{
		// �s�N�Z���P�ʂł̍��W
		float x;
		float y;

		// �s�N�Z���P�ʂł̕��ƍ���
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