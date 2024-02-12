#pragma once

#include "../Framework/GameFramework.h"

namespace DX12Wrapper
{
	class RenderTarget;
}

namespace FlappyBird
{
	/// <summary>
	/// ゲームオブジェクトを描画するパス
	/// </summary>
	class RenderObjectPass
	{
	public:
		RenderObjectPass();
		~RenderObjectPass();

		Utility::RESULT Init();

		/// <summary>
		/// ゲームオブジェクトを描画する
		/// </summary>
		/// <param name="gameObjects">ゲームオブジェクト</param>
		void Render(const std::list<std::shared_ptr<Framework::GameObject>>& gameObjects);
		
		/// <summary>
		/// ゲームオブジェクトの描画結果を取得する
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