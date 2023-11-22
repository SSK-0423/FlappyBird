#pragma once
#include "IComponent.h"

#include <DirectXMath.h>

namespace DX12Wrapper
{
	class ConstantBuffer;
}

namespace Framework
{
	class Camera : public IComponent
	{
	public:
		Camera(Object* owner);
		~Camera() = default;

		void Update(float deltaTime) override;
		void Draw() override;

		DX12Wrapper::ConstantBuffer& GetConstantBuffer() const;
		float GetFar() const;
		float GetNear() const;
		DirectX::XMMATRIX GetViewMatrix() const;
		DirectX::XMMATRIX GetProjectionMatrix() const;

	private:

		struct CameraData
		{
			DirectX::XMMATRIX view;
			DirectX::XMMATRIX projection;
		};
		CameraData m_bufferData;

		std::unique_ptr<DX12Wrapper::ConstantBuffer> m_cameraBuffer;
		float m_near = 0.f;
		float m_far = 1.f;
	};
}

///
/// カメラの実装
/// SpriteコンポーネントのDescriptorHeapにカメラのビュー行列、プロジェクション行列を有したコンスタントバッファーを設定する必要がある
/// 1. どうやってカメラのその情報を取得するか
/// 2. カメラはシーンクラスが持つのか
/// 3. オブジェクトからどうやってシーンクラスが持っているカメラを取得するのか
/// 4. カメラの情報をどうやってオブジェクトに渡すのか
/// 5. UIかどうかでカメラ情報を渡すか否かを判断する必要がある
/// 
/// 