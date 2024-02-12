#pragma once

#include "../Framework/GameFramework.h"

namespace DX12Wrapper
{
	class RenderTarget;
}

namespace FlappyBird
{
	/// <summary>
	/// �Q�[���I�u�W�F�N�g��`�悷��p�X
	/// </summary>
	class RenderObjectPass
	{
	public:
		RenderObjectPass();
		~RenderObjectPass();

		Utility::RESULT Init();

		/// <summary>
		/// �Q�[���I�u�W�F�N�g��`�悷��
		/// </summary>
		/// <param name="gameObjects">�Q�[���I�u�W�F�N�g</param>
		void Render(const std::list<std::shared_ptr<Framework::GameObject>>& gameObjects);
		
		/// <summary>
		/// �Q�[���I�u�W�F�N�g�̕`�挋�ʂ��擾����
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