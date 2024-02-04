#include "pch.h"
#include "Material.h"

#include "imgui.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"
#include "DX12Wrapper/ConstantBuffer.h"

using namespace DX12Wrapper;
using namespace Utility;

namespace Framework
{
	Material::Material(Object* owner) : IComponent(owner), m_materialData()
	{
		m_constantBuffer = std::make_shared<ConstantBuffer>();

		ID3D12Device& device = Dx12GraphicsEngine::Device();
		if (m_constantBuffer->Create(device, &m_materialData, sizeof(MaterialData)) == RESULT::FAILED)
		{
			MessageBoxA(NULL, "ContantBuffer‚Ì¶¬‚ÉŽ¸”s", "ƒGƒ‰[", MB_OK);
		}
	}
	void Material::Update(float deltaTime)
	{
		m_constantBuffer->UpdateData(&m_materialData);
	}
	void Material::Draw()
	{
	}
	void Material::DrawInspector()
	{
		if (ImGui::CollapsingHeader("Material"))
		{
			ImGui::ColorEdit4("Color", &m_materialData.color.x);
		}
	}
	void Material::SetColor(const DirectX::XMFLOAT4& color)
	{
		m_materialData.color = color;
	}
	DX12Wrapper::ConstantBuffer& Material::GetConstantBuffer()
	{
		return *m_constantBuffer.get();
	}
}