#include "pch.h"
#include "FontRenderer.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"
#include "DX12Wrapper/DescriptorHeapCBV_SRV_UAV.h"

namespace DX12Wrapper
{
	// �ÓI�����o�ϐ��̎��̉�
	Microsoft::WRL::ComPtr<ID3D12Fence> FontRenderer::m_fence = nullptr;
	UINT64 FontRenderer::m_fenceVal = 0;
	std::unique_ptr<class DescriptorHeapCBV_SRV_UAV> FontRenderer::m_fontHeap = nullptr;
	std::unique_ptr<DirectX::SpriteBatch> FontRenderer::m_spriteBatch = nullptr;
	std::unique_ptr<DirectX::SpriteFont> FontRenderer::m_spriteFont = nullptr;

	Utility::RESULT FontRenderer::Init(const std::wstring& fontPath)
	{
		auto& graphicsEngine = DX12Wrapper::Dx12GraphicsEngine::Instance();
		auto& device = graphicsEngine.Device();
		auto& cmdQueue = graphicsEngine.CmdQueue();

		// �t�F���X������
		HRESULT hresult = device.CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.ReleaseAndGetAddressOf()));
		if (FAILED(hresult))
		{
			return Utility::RESULT::FAILED;
		}

		// SpriteFont������
		m_fontHeap = std::make_unique<DescriptorHeapCBV_SRV_UAV>();
		Utility::RESULT result = m_fontHeap->Create(device);
		if (result == Utility::RESULT::FAILED)
		{
			return result;
		}

		// SpriteBatch������
		DirectX::ResourceUploadBatch resourceUpload(&device);
		resourceUpload.Begin();
		DirectX::RenderTargetState renderTargetState(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_D32_FLOAT);
		DirectX::SpriteBatchPipelineStateDescription pipelineStateDesc(renderTargetState);

		m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(&device, resourceUpload, pipelineStateDesc);

		m_spriteFont = std::make_unique<DirectX::SpriteFont>(
			&device, resourceUpload,
			fontPath.c_str(),
			m_fontHeap->GetCPUDescriptorHandleForHeapStart(),
			m_fontHeap->GetGPUDescriptorHandleForHeapStart());

		// ���\�[�X�A�b�v���[�h
		auto uploadResourcesFinished = resourceUpload.End(&cmdQueue);

		// ���\�[�X�A�b�v���[�h�����҂�
		cmdQueue.Signal(m_fence.Get(), ++m_fenceVal);
		if (m_fence->GetCompletedValue() != m_fenceVal) {
			// �C�x���g�n���h���擾
			auto event = CreateEvent(nullptr, false, false, nullptr);

			m_fence->SetEventOnCompletion(m_fenceVal, event);

			// �C�x���g����������܂ő҂�������
			WaitForSingleObject(event, INFINITE);

			// �C�x���g�n���h�������
			CloseHandle(event);
		}
		uploadResourcesFinished.wait();

		m_spriteBatch->SetViewport(graphicsEngine.GetViewport());

		return result;
	}
	void FontRenderer::DrawString(const std::wstring& text, const DirectX::XMFLOAT2& pos, const float& scale, const DirectX::XMVECTORF32& color)
	{
		auto& renderContext = Dx12GraphicsEngine::Instance().GetRenderingContext();

		renderContext.SetDescriptorHeap(*m_fontHeap.get());
		m_spriteBatch->Begin(&Dx12GraphicsEngine::Instance().CmdList());
		m_spriteFont->DrawString(
			m_spriteBatch.get(),			// SpriteBatch
			text.c_str(),					// ������
			pos,							// ���W
			color,							// �F
			0.f,							// ��]�p�x
			DirectX::XMFLOAT2(0.f, 0.f),	// ��]��
			scale);							// �g�嗦
		m_spriteBatch->End();
	}
}