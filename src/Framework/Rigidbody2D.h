#pragma once

#include "IComponent.h"

#include <DirectXMath.h>

namespace Framework
{
	enum class FORCE_MODE
	{
		ACCELERATION,
		VELOCITY,
		FORCE,
	};

	class Rigidbody2D : public IComponent
	{
	public:
		Rigidbody2D(std::shared_ptr<Object> owner);
		~Rigidbody2D();

		void Update(float deltaTime) override;
		void FixedUpdate(float interval) override;
		void Draw() override;

		void AddForce(DirectX::XMFLOAT2 force, FORCE_MODE mode = FORCE_MODE::FORCE);

		float mass = 1.f;
		float gravity = 9.8f;
		float gravityScale = 1.f;
		bool useGravity = true;

		DirectX::XMFLOAT2 velocity = { 0.f, 0.f };
		DirectX::XMFLOAT2 acceleration = { 0.f, 0.f };
	};
}