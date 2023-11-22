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
		// Transform2D側でピクセル単位の数値を正規化デバイス座標系(-1～1)に変換しているので、
		// World変換後の数値をそのまま出力するためにプロジェクション行列のXYスケール成分は2にする
		m_bufferData.projection = XMMatrixOrthographicLH(2.f, 2.f, m_near, m_far);

		auto transform = m_owner->GetComponent<Transform2D>();
		// カメラを画面の中心に配置
		transform->position = { windowSize.cx / 2.f, windowSize.cy / 2.f };
		transform->angle = 0.f;

		// Transform2D側でピクセル単位の数値を正規化デバイス座標系(-1～1)に変換する際に、
		// スケール成分に 2.f / ウィンドウサイズ をかけているので、
		// スケール成分を1とするためにここではウィンドウサイズの半分を設定する
		transform->scale = { windowSize.cx / 2.f, windowSize.cy / 2.f };
		m_bufferData.view = transform->GetTransformMatrix();

		auto& device = Dx12GraphicsEngine::Instance().Device();
		if (m_cameraBuffer->Create(device, &m_bufferData, sizeof(CameraData)) == RESULT::FAILED)
		{
			MessageBoxA(NULL, "ContantBufferの生成に失敗", "エラー", MB_OK);
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

