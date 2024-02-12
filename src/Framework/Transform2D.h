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
	/// �A�t�B���ϊ��s����Ǘ�����R���|�[�l���g
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
		/// ���f���s����Ǘ�����\����
		/// ���̍\���̂̓V�F�[�_�[���Ɠ����\���̂�����
		/// </summary>
		struct Transform2DData
		{
			DirectX::XMMATRIX model;
		};
		Transform2DData m_bufferData;	// �V�F�[�_�[�ɓn���f�[�^
		std::shared_ptr<DX12Wrapper::ConstantBuffer> m_transformBuffer;	// ���f���s����i�[����o�b�t�@
	};
}