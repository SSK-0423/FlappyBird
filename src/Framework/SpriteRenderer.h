#pragma once
#include "IComponent.h"

#include <d3d12.h>
#include <DirectXMath.h>

namespace DX12Wrapper
{
	class RootSignature;
	class GraphicsPipelineState;
	class ConstantBuffer;
}

namespace Framework
{
	enum class SPRITE_DRAW_MODE
	{
		GAMEOBJECT,
		GUI,
	};

	class SpriteRenderer : public IComponent
	{
	public:
		SpriteRenderer(std::shared_ptr<Object> owner);
		~SpriteRenderer();

		void Start() override;
		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		// TODO: スプライトアニメーションを実装する
		void SetSprite(class Sprite* sprite);
		void SetSprite(std::shared_ptr<class Sprite> sprite);

		void AddSprite(class Sprite* sprite);
		void AddSprite(std::shared_ptr<class Sprite> sprite);

		void ChangeRenderSprite(size_t index);

		void SetDrawMode(SPRITE_DRAW_MODE drawMode);

		/// <summary>
		/// スプライトの描画順を制御するレイヤー(最大32)を設定する
		/// レイヤーの値が小さいほど手前に描画される
		/// </summary>
		/// <param name="layer">レイヤー</param>
		void SetLayer(UINT layer);

		Utility::RESULT CreateGraphicsPipelineState(ID3D12Device& device);
		Utility::RESULT CreateRootSignature(ID3D12Device& device);

	private:
		enum class CONSTANT_BUFFER_INDEX
		{
			TRANSFORM = 0,
			CAMERA = 1,
			DRAW_MODE = 2,
			MATERIAL = 3,
			BUFFER_COUNT
		};

		std::shared_ptr<DX12Wrapper::RootSignature> m_rootSignature = nullptr;
		std::shared_ptr<DX12Wrapper::GraphicsPipelineState> m_pipelineState = nullptr;
		std::shared_ptr<DX12Wrapper::ConstantBuffer> m_drawModeBuffer = nullptr;
		std::vector<std::shared_ptr<class Sprite>> m_sprites;

		size_t m_currentSpriteIndex = 0;
		UINT m_layer = 0;

		SPRITE_DRAW_MODE m_drawMode = SPRITE_DRAW_MODE::GAMEOBJECT;
	};
}