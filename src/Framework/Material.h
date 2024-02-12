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
	/// �}�e���A���R���|�[�l���g
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
		/// �}�e���A���̐F��ݒ肷��
		/// </summary>
		/// <param name="color"></param>
		void SetColor(const DirectX::XMFLOAT4& color);

		DX12Wrapper::ConstantBuffer& GetConstantBuffer();

	private:
		/// <summary>
		/// �}�e���A���̒萔�o�b�t�@�ɓn���\����
		/// </summary>
		struct MaterialData
		{
			DirectX::XMFLOAT4 color;

			MaterialData() : color(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)) {}
		};
		MaterialData m_materialData;	// �V�F�[�_�[�ɓn���f�[�^

		std::shared_ptr<DX12Wrapper::ConstantBuffer> m_constantBuffer = nullptr;	// �}�e���A���̒萔�o�b�t�@
	};
}