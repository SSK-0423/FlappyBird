#include "pch.h"
#include "Rigidbody2D.h"

#include "Object.h"
#include "Transform2D.h"

#include "../Utility/EngineUtility.h"

using namespace DirectX;

namespace Framework
{
	Rigidbody2D::Rigidbody2D(Object* owner)
		: IComponent(owner)
	{
	}
	Rigidbody2D::~Rigidbody2D()
	{
	}
	void Rigidbody2D::Update(float deltaTime)
	{
		velocity.x += acceleration.x * deltaTime;
		velocity.y += acceleration.y * deltaTime;

		// 重力が有効なら適用する
		if (useGravity)
		{
			// Vf = Vi + at
			velocity.y += gravity * gravityScale * deltaTime;
		}

		// 適用
		XMFLOAT2 position = m_owner->GetComponent<Transform2D>()->position;
		position.x += velocity.x;
		position.y += velocity.y;
		m_owner->GetComponent<Transform2D>()->position = position;

		Utility::DebugLog("Velocity: %f\n", velocity.y);
	}
	void Rigidbody2D::Draw()
	{
	}
	void Rigidbody2D::AddForce(DirectX::XMFLOAT2 force, FORCE_MODE mode)
	{
		switch (mode)
		{
		case FORCE_MODE::FORCE:
		{
			acceleration.x += force.x / mass;
			acceleration.y += force.y / mass;
			break;
		}
		case FORCE_MODE::VELOCITY:
		{
			velocity.x += force.x;
			velocity.y += force.y;
		}
		}
	}
}
