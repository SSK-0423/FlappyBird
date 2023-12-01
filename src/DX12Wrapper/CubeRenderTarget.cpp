#include "CubeRenderTarget.h"

#include "DescriptorHeapRTV.h"
#include "DescriptorHeapCBV_SRV_UAV.h"
#include "RenderTargetBuffer.h"

#include "DescriptorHeapDSV.h"
#include "DepthStencilBuffer.h"

#include "Texture.h"
#include "ShaderResourceViewDesc.h"

using namespace Utility;

namespace DX12Wrapper
{
	RESULT CubeRenderTarget::Create(ID3D12Device& device, CubeRenderTargetData& renderTargetData)
	{
		m_renderTargetData = renderTargetData;

		// レンダーターゲット生成
		RESULT result = m_renderTargetBuffer.Create(device, renderTargetData.renderTargetBufferData);
		if (result == RESULT::FAILED) { return result; }

		// レンダーターゲットヒープ生成
		result = m_rtvHeap.Create(device);
		if (result == RESULT::FAILED) { return result; }

		// レンダーターゲットビュー生成
		m_rtvHeap.RegistDescriptor(
			device, m_renderTargetBuffer, m_renderTargetData.renderTargetBufferData.colorFormat);

		// オフスクリーンテクスチャバッファー生成
		m_renderTargetTexture.CreateTextureFromRenderTarget(m_renderTargetBuffer);

		// テクスチャ用ヒープ生成
		result = m_textureHeap.Create(device);
		if (result == RESULT::FAILED) { return result; }

		// テクスチャとして登録
		ShaderResourceViewDesc renderDesc(m_renderTargetTexture);
		m_textureHeap.RegistShaderResource(device, m_renderTargetTexture, renderDesc);

		// デプスステンシルバッファー生成
		result = m_depthStencilBuffer.Create(device, renderTargetData.depthStencilBufferData);
		if (result == RESULT::FAILED) { return result; }

		// デプスステンシル用ディスクリプタヒープ生成
		result = m_dsvHeap.Create(device);
		if (result == RESULT::FAILED) { return result; }

		// デプスステンシルビュー生成
		m_dsvHeap.RegistDescriptor(device, m_depthStencilBuffer);

		// デプスステンシルテクスチャバッファー生成
		m_depthStencilTexture.CreateTextureFromDepthStencil(m_depthStencilBuffer);

		// テクスチャとして登録
		ShaderResourceViewDesc depthDesc(m_depthStencilTexture);
		m_textureHeap.RegistShaderResource(device, m_depthStencilTexture, depthDesc);

		return RESULT::SUCCESS;
	}

	void CubeRenderTarget::BeginRendering(RenderingContext& renderContext, CD3DX12_VIEWPORT& viewport, CD3DX12_RECT& scissorRect)
	{
		// レンダーターゲットに移行
		renderContext.TransitionResourceState(
			m_renderTargetBuffer.GetBuffer(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET);

		// レンダーターゲットセット
		auto rtvHandle = m_rtvHeap.GetCPUDescriptorHandleForHeapStart();

		if (m_renderTargetData.useDepth) {

			// 深度バッファー
			auto dsvHandle = m_dsvHeap.GetCPUDescriptorHandleForHeapStart();

			renderContext.SetRenderTarget(&rtvHandle, &dsvHandle);

			// デプスステンシルバッファーをクリア
			renderContext.ClearDepthStencilView(
				dsvHandle, D3D12_CLEAR_FLAG_DEPTH,
				m_renderTargetData.depthStencilBufferData.clearDepth,
				m_renderTargetData.depthStencilBufferData.clearStencil, 0, nullptr);
		}
		else {
			renderContext.SetRenderTarget(&rtvHandle, nullptr);
		}

		// 画面を指定色でクリア
		renderContext.ClearRenderTarget(rtvHandle, m_renderTargetData.renderTargetBufferData.clearColor, 0, nullptr);

		// ビューポート、シザー矩形セット
		renderContext.SetViewport(viewport);
		renderContext.SetScissorRect(scissorRect);
	}

	void CubeRenderTarget::EndRendering(RenderingContext& renderContext)
	{
		// ピクセルシェーダーリソースへ移行
		renderContext.TransitionResourceState(
			m_renderTargetBuffer.GetBuffer(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	}

	void CubeRenderTarget::BeginMultiRendering(
		CubeRenderTarget* renderTargets, const size_t& length, RenderingContext& renderContext,
		CD3DX12_VIEWPORT& viewport, CD3DX12_RECT& scissorRect)
	{
		// ハンドル
		D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandles = new D3D12_CPU_DESCRIPTOR_HANDLE[length];
		D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandle = nullptr;

		for (size_t idx = 0; idx < length; idx++)
		{
			// レンダーターゲットに移行
			renderContext.TransitionResourceState(
				renderTargets[idx].m_renderTargetBuffer.GetBuffer(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET);

			rtvHandles[idx] = renderTargets[idx].m_rtvHeap.GetCPUDescriptorHandleForHeapStart();

			// 画面を指定色でクリア
			renderContext.ClearRenderTarget(
				rtvHandles[idx], renderTargets[idx].m_renderTargetData.renderTargetBufferData.clearColor, 0, nullptr);

			// デプスステンシルは1つしかセットできないので、最初に見つかったものを使用する
			if (renderTargets[idx].m_renderTargetData.useDepth && dsvHandle == nullptr) {
				dsvHandle = new D3D12_CPU_DESCRIPTOR_HANDLE();
				auto handle = renderTargets[idx].m_dsvHeap.GetCPUDescriptorHandleForHeapStart();
				// GetCPUDescriptorHandleForHeapStartの戻り値が値渡しなので、
				// スコープを抜ける際にdsvHandleがnullとなるのを防ぐためにコピーする
				memcpy_s(
					dsvHandle,
					sizeof(D3D12_CPU_DESCRIPTOR_HANDLE),
					&handle,
					sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));

				// デプスステンシルバッファーをクリア
				renderContext.ClearDepthStencilView(
					*dsvHandle, D3D12_CLEAR_FLAG_DEPTH,
					renderTargets[idx].m_renderTargetData.depthStencilBufferData.clearDepth,
					renderTargets[idx].m_renderTargetData.depthStencilBufferData.clearStencil, 0, nullptr);
			}
		}

		// レンダーターゲットセット
		renderContext.SetRenderTargets(length, rtvHandles, dsvHandle);

		// ビューポート、シザー矩形セット
		renderContext.SetViewport(viewport);
		renderContext.SetScissorRect(scissorRect);
	}

	void CubeRenderTarget::EndMultiRendering(CubeRenderTarget* renderTargets, const size_t& length, RenderingContext& renderContext)
	{
		for (size_t idx = 0; idx < length; idx++)
		{
			// ピクセルシェーダーリソースへ移行
			renderContext.TransitionResourceState(
				renderTargets[idx].m_renderTargetBuffer.GetBuffer(),
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		}
	}
}