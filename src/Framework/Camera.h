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
/// �J�����̎���
/// Sprite�R���|�[�l���g��DescriptorHeap�ɃJ�����̃r���[�s��A�v���W�F�N�V�����s���L�����R���X�^���g�o�b�t�@�[��ݒ肷��K�v������
/// 1. �ǂ�����ăJ�����̂��̏����擾���邩
/// 2. �J�����̓V�[���N���X�����̂�
/// 3. �I�u�W�F�N�g����ǂ�����ăV�[���N���X�������Ă���J�������擾����̂�
/// 4. �J�����̏����ǂ�����ăI�u�W�F�N�g�ɓn���̂�
/// 5. UI���ǂ����ŃJ��������n�����ۂ��𔻒f����K�v������
/// 
/// 