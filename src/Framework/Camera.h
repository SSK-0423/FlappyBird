#pragma once
#include "IComponent.h"

#include <DirectXMath.h>

namespace DX12Wrapper
{
	class ConstantBuffer;
}

namespace Framework
{
	/// <summary>
	/// カメラコンポーネント
	/// </summary>
	class Camera : public IComponent
	{
	public:
		Camera(std::shared_ptr<Object> owner);
		~Camera();

		void Update(float deltaTime) override;
		void Draw() override;

		DX12Wrapper::ConstantBuffer& GetConstantBuffer() const;
		float GetFar() const;
		float GetNear() const;
		DirectX::XMMATRIX GetViewMatrix() const;
		DirectX::XMMATRIX GetProjectionMatrix() const;

	private:

		/// <summary>
		/// カメラの定数バッファに渡す構造体
		/// </summary>
		struct CameraData
		{
			DirectX::XMMATRIX view;
			DirectX::XMMATRIX projection;
		};
		CameraData m_bufferData;	                                    // シェーダーに渡すデータ

		std::unique_ptr<DX12Wrapper::ConstantBuffer> m_cameraBuffer;	// カメラの定数バッファ
		float m_near = 0.f;
		float m_far = 1.f;
	};
}