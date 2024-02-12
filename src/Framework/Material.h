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
	/// マテリアルコンポーネント
	/// </summary>
	class Material : public IComponent
	{
	public:
		Material(std::shared_ptr<Object> owner);
		~Material() = default;

		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		/// <summary>
		/// マテリアルの色を設定する
		/// </summary>
		/// <param name="color"></param>
		void SetColor(const DirectX::XMFLOAT4& color);

		DX12Wrapper::ConstantBuffer& GetConstantBuffer();

	private:
		/// <summary>
		/// マテリアルの定数バッファに渡す構造体
		/// </summary>
		struct MaterialData
		{
			DirectX::XMFLOAT4 color;

			MaterialData() : color(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)) {}
		};
		MaterialData m_materialData;	// シェーダーに渡すデータ

		std::shared_ptr<DX12Wrapper::ConstantBuffer> m_constantBuffer = nullptr;	// マテリアルの定数バッファ
	};
}