#pragma once

#include "../Framework/GameFramework.h"

namespace DX12Wrapper
{
	class RenderTarget;
}

namespace FlappyBird
{
	class RenderUIPass
	{
	public:
		RenderUIPass();
		~RenderUIPass();

		Utility::RESULT Init();

		void Render(const std::list<std::shared_ptr<Framework::UIObject>>& uiObjects);
		void SetRenderTexture(DX12Wrapper::Texture& texture);
		DX12Wrapper::Texture& GetRenderTexture();

	private:
		CD3DX12_VIEWPORT m_viewport;
		CD3DX12_RECT m_scissorRect;

		std::unique_ptr<DX12Wrapper::RenderTarget> m_renderTarget = nullptr;
		Utility::RESULT CreateRenderTarget(ID3D12Device& device);
	};
}