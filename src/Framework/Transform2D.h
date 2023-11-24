#pragma once
#include "IComponent.h"
#include <DirectXMath.h>

namespace DX12Wrapper
{
	class ConstantBuffer;
}

namespace Framework
{
	class Transform2D : public IComponent
	{
	public:
		Transform2D(Object* owner);
		~Transform2D();

	private:
		struct Transform2DData
		{
			DirectX::XMMATRIX model;
		};
		Transform2DData m_bufferData;
		std::shared_ptr<DX12Wrapper::ConstantBuffer> m_transformBuffer;


	public:
		void Update(float deltaTime) override;
		void Draw() override {};

		DirectX::XMFLOAT2 position;
		DirectX::XMFLOAT2 scale;
		float angle;
		float depth;

		DX12Wrapper::ConstantBuffer& GetConstantBuffer();
		DirectX::XMMATRIX GetTransformMatrix();
	};
}