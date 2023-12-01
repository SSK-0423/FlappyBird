#pragma once

#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "../Utility/EngineUtility.h"

namespace DX12Wrapper
{
	/// <summary>
	/// グラフィックスパイプラインステートクラス
	/// </summary>
	class GraphicsPipelineState {
	public:
		GraphicsPipelineState() = default;
		~GraphicsPipelineState() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState = nullptr;	// パイプラインステート

		D3D12_GRAPHICS_PIPELINE_STATE_DESC m_pipelineStateDesc;

		/// <summary>
		/// パイプラインステート生成
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <param name="piplineStateDesc">グラフィックスパイプラインステート構造体</param>
		/// <returns></returns>
		HRESULT CreateGraphicsPipelineState(
			ID3D12Device& device, const D3D12_GRAPHICS_PIPELINE_STATE_DESC& piplineStateDesc);

	public:
		/// <summary>
		/// パイプラインステート生成
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <param name="piplineStateDesc">グラフィックスパイプラインステート構造体</param>
		/// <returns></returns>
		Utility::RESULT Create(ID3D12Device& device, const D3D12_GRAPHICS_PIPELINE_STATE_DESC& piplineStateDesc);

		/// <summary>
		/// パイプラインステート取得
		/// </summary>
		/// <returns>パイプラインステート</returns>
		ID3D12PipelineState& GetPipelineState() {
			return *m_pipelineState.Get();
		}
	};
}