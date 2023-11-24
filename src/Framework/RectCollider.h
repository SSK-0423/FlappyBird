#pragma once

#include "Collider.h"

#include <DirectXMath.h>

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

		// ���㌴�_���ǂ���
		bool isLeftUpOrigin;

		Rect(float x, float y, float width, float height, bool isLeftUpOrigin)
			: x(x), y(y), width(width), height(height), isLeftUpOrigin(isLeftUpOrigin)
		{}

		DirectX::XMFLOAT2 GetCenter() const
		{
			// ���オ���_�̏ꍇ
			if (isLeftUpOrigin)
				return DirectX::XMFLOAT2(x + width / 2, y - height / 2);
			// ���S�����_�̏ꍇ
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