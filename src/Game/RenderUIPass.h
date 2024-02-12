#pragma once

#include "../Framework/GameFramework.h"

namespace DX12Wrapper
{
	class RenderTarget;
}

namespace FlappyBird
{
	/// <summary>
	/// UI�I�u�W�F�N�g��`�悷��p�X
	/// </summary>
	class RenderUIPass
	{
	public:
		RenderUIPass();
		~RenderUIPass();

		Utility::RESULT Init();

		void Render(const std::list<std::shared_ptr<Framework::UIObject>>& uiObjects);
		
		/// <summary>
		/// �Q�[���I�u�W�F�N�g�̕`�挋�ʂ��Z�b�g����
		/// </summary>
		/// <param name="texture">�Q�[���I�u�W�F�N�g�̕`�挋�ʂ��i�[�����e�N�X�`��</param>
		void SetRenderTexture(DX12Wrapper::Texture& texture);

		/// <summary>
		/// �`�挋�ʂ��擾����
		/// </summary>
		/// <returns></returns>
		DX12Wrapper::Texture& GetRenderTexture();

	private:
		CD3DX12_VIEWPORT m_viewport;
		CD3DX12_RECT m_scissorRect;

		std::unique_ptr<DX12Wrapper::RenderTarget> m_renderTarget = nullptr;
		Utility::RESULT CreateRenderTarget(ID3D12Device& device);
	};
}