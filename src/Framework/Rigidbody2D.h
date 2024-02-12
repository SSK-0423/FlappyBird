#pragma once

#include "IComponent.h"

#include <DirectXMath.h>

namespace Framework
{
	/// <summary>
	/// 力の種類
	/// </summary>
	enum class FORCE_MODE
	{
		ACCELERATION,
		VELOCITY,
		FORCE,
	};

	/// <summary>
	/// 2Dの剛体の物理演算を行うコンポーネント
	/// </summary>
	class Rigidbody2D : public IComponent
	{
	public:
		Rigidbody2D(std::shared_ptr<Object> owner);
		~Rigidbody2D();

		void Update(float deltaTime) override;
		void FixedUpdate(float interval) override;
		void Draw() override;
		void DrawInspector() override;

		/// <summary>
		/// このコンポーネントがアタッチされているオブジェクトに力を加える
		/// </summary>
		/// <param name="force">力のベクトル量</param>
		/// <param name="mode">力の種類</param>
		void AddForce(DirectX::XMFLOAT2 force, FORCE_MODE mode = FORCE_MODE::FORCE);

		float mass = 1.f;		                        // 質量
		float gravity = 9.8f;	                        // 重力加速度
		float gravityScale = 1.f;	                    // 重力のスケール
		bool useGravity = true;	                        // 重力を使用するかどうか

		DirectX::XMFLOAT2 velocity = { 0.f, 0.f };	    // 速度
		DirectX::XMFLOAT2 acceleration = { 0.f, 0.f };	// 加速度
	};
}