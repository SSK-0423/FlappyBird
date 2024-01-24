#pragma once

#include "../Framework/GameFramework.h"

namespace DX12Wrapper
{
	class RootSignature;
	class GraphicsPipelineState;
	class DescriptorHeapCBV_SRV_UAV;
	class ShaderResourceViewDesc;
	class Texture;
}

namespace FlappyBird
{
	class RenderScreenPass
	{
	public:
		RenderScreenPass();
		~RenderScreenPass();

		Utility::RESULT Init();

		void Render();

		void SetRenderTexture(DX12Wrapper::ShaderResourceViewDesc& desc, DX12Wrapper::Texture& texture);

	private:
		CD3DX12_VIEWPORT m_viewport;
		CD3DX12_RECT m_scissorRect;

		std::unique_ptr<DX12Wrapper::RootSignature> m_rootSignature = nullptr;
		std::unique_ptr<DX12Wrapper::GraphicsPipelineState> m_pipelineState = nullptr;
		std::unique_ptr<DX12Wrapper::DescriptorHeapCBV_SRV_UAV> m_descriptorHeap = nullptr;

		Utility::RESULT CreateRootSignature();
		Utility::RESULT CreatePipelineState();
		Utility::RESULT CreateDescriptorHeap();
	};
}