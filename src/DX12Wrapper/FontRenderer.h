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
		static Utility::RESULT Init(const std::wstring& fontPath);
		static void DrawString(
			const std::wstring& text, const DirectX::XMFLOAT2& pos,
			const float& scale, const DirectX::XMVECTORF32& color);
	private:
		FontRenderer() = default;
		~FontRenderer() = default;

		static Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
		static UINT64 m_fenceVal;
		static std::unique_ptr<class DescriptorHeapCBV_SRV_UAV> m_fontHeap;
		static std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
		static std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	};
}