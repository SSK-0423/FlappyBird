#pragma once

#include "IComponent.h"
#include <DirectXMath.h>

namespace DX12Wrapper
{
	class ConstantBuffer;
}

namespace Framework
{
	class Material : public IComponent
	{
	public:
		Material(std::shared_ptr<Object> owner);
		~Material() = default;

		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		void SetColor(const DirectX::XMFLOAT4& color);

		DX12Wrapper::ConstantBuffer& GetConstantBuffer();

	private:
		struct MaterialData
		{
			DirectX::XMFLOAT4 color;

			MaterialData() : color(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)) {}
		};
		MaterialData m_materialData;

		std::shared_ptr<DX12Wrapper::ConstantBuffer> m_constantBuffer = nullptr;
	};
}