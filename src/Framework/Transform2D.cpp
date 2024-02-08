#include "pch.h"
#include "Transform2D.h"
#include "DX12Wrapper/ConstantBuffer.h"
#include "DX12Wrapper/Dx12GraphicsEngine.h"

#include "Framework/Window.h"

#include "Object.h"

#include "imgui.h"

using namespace DX12Wrapper;
using namespace Utility;
using namespace DirectX;

namespace Framework
{
	Transform2D::Transform2D(std::shared_ptr<Object> owner)
		: IComponent(owner), m_transformBuffer(std::make_shared<ConstantBuffer>()),
		position(0.f, 0.f), scale(100.f, 100.f), angle(0.f), depth(0.f)
	{
		RESULT result = m_transformBuffer->Create(Dx12GraphicsEngine::Device(), &m_bufferData, sizeof(m_bufferData));
		if (result == RESULT::FAILED)
		{
			MessageBoxA(NULL, "ContantBufferの生成に失敗", "エラー", MB_OK);
		}
	}
	Transform2D::~Transform2D()
	{
		OutputDebugStringA("Transform2D::~Transform2D()\n");
	}
	void Transform2D::Update(float deltaTime)
	{
		m_bufferData.model = GetTransformMatrix();
		m_transformBuffer->UpdateData(&m_bufferData);
	}
	void Transform2D::DrawInspector()
	{
		if (ImGui::CollapsingHeader("Transform2D"))
		{
			// Transform2Dの値を表示
			ImGui::PushItemWidth(200);
			ImGui::InputFloat2("Position", &position.x);
			ImGui::InputFloat("Rotation", &angle);
			ImGui::InputFloat2("Scale   ", &scale.x);
		}
	}
	DirectX::XMMATRIX Transform2D::GetTransformMatrix()
	{
		// デバイス座標系に合わせる際に+1するのでy座標はマイナス値を使う y=0⇒1 y=windowHeight⇒-1
		XMMATRIX translation = XMMatrixTranslation(position.x, -position.y, depth);
		XMMATRIX scaling = XMMatrixScaling(scale.x, scale.y, 1.f);
		XMMATRIX rotation = XMMatrixRotationZ(XMConvertToRadians(angle));

		// ピクセル単位の座標系から正規化デバイス座標系に変更する行列
		XMMATRIX convertToDeviceCoord = XMMatrixIdentity();
		auto viewportSize = Dx12GraphicsEngine::GetViewport();
		convertToDeviceCoord.r[0].m128_f32[0] = 2.f / viewportSize.Width;	// (0～1)に正規化
		convertToDeviceCoord.r[1].m128_f32[1] = 2.f / viewportSize.Height;	// (0～1)に正規化
		convertToDeviceCoord.r[3].m128_f32[0] = -1.f;	//デバイス座標系(-1～1)に合わせる
		convertToDeviceCoord.r[3].m128_f32[1] = 1.f;	//デバイス座標系(-1～1)に合わせる

		XMMATRIX transformMatrix = scaling * rotation * translation * convertToDeviceCoord;

		return transformMatrix;
	}
	DX12Wrapper::ConstantBuffer& Transform2D::GetConstantBuffer()
	{
		return *m_transformBuffer;
	}
}

