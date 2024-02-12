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
	/// �J�����R���|�[�l���g
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
		/// �J�����̒萔�o�b�t�@�ɓn���\����
		/// </summary>
		struct CameraData
		{
			DirectX::XMMATRIX view;
			DirectX::XMMATRIX projection;
		};
		CameraData m_bufferData;	                                    // �V�F�[�_�[�ɓn���f�[�^

		std::unique_ptr<DX12Wrapper::ConstantBuffer> m_cameraBuffer;	// �J�����̒萔�o�b�t�@
		float m_near = 0.f;
		float m_far = 1.f;
	};
}