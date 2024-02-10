#include "pch.h"
#include "Rigidbody2D.h"

#include "Object.h"
#include "Transform2D.h"

#include "../Utility/EngineUtility.h"

using namespace DirectX;

namespace Framework
{
	Rigidbody2D::Rigidbody2D(std::shared_ptr<Object> owner)
		: IComponent(owner)
	{
	}
	Rigidbody2D::~Rigidbody2D()
	{
	}
	void Rigidbody2D::Update(float deltaTime)
	{
	}
	void Rigidbody2D::FixedUpdate(float interval)
	{
		velocity.x += acceleration.x;
		velocity.y += acceleration.y;

		// 重力が有効なら適用する
		if (useGravity)
		{
			// Vf = Vi + at
			velocity.y += gravity * gravityScale;
		}

		// 適用
		XMFLOAT2 position = m_owner.lock()->GetComponent<Transform2D>()->position;
		position.x += velocity.x * interval;
		position.y += velocity.y * interval;
		m_owner.lock()->GetComponent<Transform2D>()->position = position;
	}
	void Rigidbody2D::Draw()
	{
	}
	void Rigidbody2D::DrawInspector()
	{
		if (ImGui::CollapsingHeader("Rigidbody2D"))
		{
			ImGui::InputFloat("GravityScale", &gravityScale);
		}
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
