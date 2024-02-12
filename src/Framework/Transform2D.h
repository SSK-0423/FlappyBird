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
	/// アフィン変換行列を管理するコンポーネント
	/// </summary>
	class Transform2D : public IComponent
	{
	public:
		Transform2D(std::shared_ptr<Object> owner);
		~Transform2D();

		void Update(float deltaTime) override;
		void Draw() override {};
		void DrawInspector() override;

		DirectX::XMFLOAT2 position;
		DirectX::XMFLOAT2 scale;
		float angle;
		float depth;

		DX12Wrapper::ConstantBuffer& GetConstantBuffer();
		DirectX::XMMATRIX GetTransformMatrix();

	private:
		/// <summary>
		/// モデル行列を管理する構造体
		/// この構造体はシェーダー側と同じ構造体を持つ
		/// </summary>
		struct Transform2DData
		{
			DirectX::XMMATRIX model;
		};
		Transform2DData m_bufferData;	// シェーダーに渡すデータ
		std::shared_ptr<DX12Wrapper::ConstantBuffer> m_transformBuffer;	// モデル行列を格納するバッファ
	};
}