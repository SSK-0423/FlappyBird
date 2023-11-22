#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <SpriteFont.h>
#include <ResourceUploadBatch.h>

#include "Utility/EngineUtility.h"

#pragma comment(lib,"DirectXTK12.lib")

namespace DX12Wrapper
{
	class FontRenderer
	{
	public:
		FontRenderer();
		~FontRenderer() = default;

		static FontRenderer& Instance();

		Utility::RESULT Init(const std::wstring& fontPath);
		void DrawString(
			const std::wstring& text, const DirectX::XMFLOAT2& pos,
			const float& scale, const DirectX::XMVECTORF32& color);
	private:
		// ƒtƒFƒ“ƒX
		Microsoft::WRL::ComPtr<ID3D12Fence> m_fence = nullptr;
		UINT64 m_fenceVal = 0;
		std::unique_ptr<class DescriptorHeapCBV_SRV_UAV> m_fontHeap = nullptr;
		std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch = nullptr;
		std::unique_ptr<DirectX::SpriteFont> m_spriteFont = nullptr;
	};
}