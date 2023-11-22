#pragma once
#include <d3d12.h>

#include "Framework/IRenderer.h"
#include "Utility/EngineUtility.h"

namespace DX12Wrapper
{
	class RootSignature;
	class GraphicsPipelineState;
}

namespace Framework
{
	class ButtonRenderer : public Framework::IRenderer
	{
	public:
		ButtonRenderer();
		~ButtonRenderer();

	private:
		std::unique_ptr<DX12Wrapper::RootSignature> m_rootSignature;
		std::unique_ptr<DX12Wrapper::GraphicsPipelineState> m_pipelineState;

		std::unique_ptr<DX12Wrapper::RootSignature> m_rootSignature2;
		std::unique_ptr<DX12Wrapper::GraphicsPipelineState> m_pipelineState2;

		Utility::RESULT CreateGraphicsPipelineState(ID3D12Device& device);
		Utility::RESULT CreateRootSignature(ID3D12Device& device);

		void Render();
	public:
		Utility::RESULT Init() override;
	};
}