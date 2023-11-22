#include "pch.h"
#include "Camera.h"

#include "Object.h"
#include "Window.h"
#include "Transform2D.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"
#include "DX12Wrapper/ConstantBuffer.h"

using namespace DX12Wrapper;
using namespace Utility;
using namespace DirectX;

namespace Framework
{
	Camera::Camera(Object* owner) : IComponent(owner)
	{
		m_cameraBuffer = std::make_unique<ConstantBuffer>();

		auto windowSize = Window::GetWindowSize();
		// Transform2D���Ńs�N�Z���P�ʂ̐��l�𐳋K���f�o�C�X���W�n(-1�`1)�ɕϊ����Ă���̂ŁA
		// World�ϊ���̐��l�����̂܂܏o�͂��邽�߂Ƀv���W�F�N�V�����s���XY�X�P�[��������2�ɂ���
		m_bufferData.projection = XMMatrixOrthographicLH(2.f, 2.f, m_near, m_far);

		auto transform = m_owner->GetComponent<Transform2D>();
		// �J��������ʂ̒��S�ɔz�u
		transform->position = { windowSize.cx / 2.f, windowSize.cy / 2.f };
		transform->angle = 0.f;

		// Transform2D���Ńs�N�Z���P�ʂ̐��l�𐳋K���f�o�C�X���W�n(-1�`1)�ɕϊ�����ۂɁA
		// �X�P�[�������� 2.f / �E�B���h�E�T�C�Y �������Ă���̂ŁA
		// �X�P�[��������1�Ƃ��邽�߂ɂ����ł̓E�B���h�E�T�C�Y�̔�����ݒ肷��
		transform->scale = { windowSize.cx / 2.f, windowSize.cy / 2.f };
		m_bufferData.view = transform->GetTransformMatrix();

		auto& device = Dx12GraphicsEngine::Instance().Device();
		if (m_cameraBuffer->Create(device, &m_bufferData, sizeof(CameraData)) == RESULT::FAILED)
		{
			MessageBoxA(NULL, "ContantBuffer�̐����Ɏ��s", "�G���[", MB_OK);
		}
	}
	void Camera::Update(float deltaTime)
	{
		m_bufferData.view = m_owner->GetComponent<Transform2D>()->GetTransformMatrix();
		m_cameraBuffer->UpdateData(&m_bufferData);
	}
	void Camera::Draw()
	{
	}
	ConstantBuffer& Framework::Camera::GetConstantBuffer() const
	{
		return *m_cameraBuffer.get();
	}
	float Camera::GetFar() const
	{
		return m_far;
	}
	float Camera::GetNear() const
	{
		return m_near;
	}
	DirectX::XMMATRIX Camera::GetViewMatrix() const
	{
		return m_bufferData.view;
	}
	DirectX::XMMATRIX Camera::GetProjectionMatrix() const
	{
		return m_bufferData.projection;
	}
}

