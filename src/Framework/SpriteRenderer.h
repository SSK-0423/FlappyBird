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
	/// <summary>
	/// スプライトの描画モード
	/// </summary>
	enum class SPRITE_DRAW_MODE
	{
		GAMEOBJECT,
		GUI,
	};

	/// <summary>
	/// スプライトを描画するコンポーネント
	/// </summary>
	class SpriteRenderer : public IComponent
	{
	public:
		SpriteRenderer(std::shared_ptr<Object> owner);
		~SpriteRenderer();

		void Start() override;
		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		static void BeginDraw();
		static void EndDraw();

		// TODO: スプライトアニメーションを実装する

		/// <summary>
		/// 描画するスプライトを設定する
		/// この関数はindex:0のスプライトを設定する
		/// </summary>
		/// <param name="sprite">スプライト</param>
		void SetSprite(class Sprite* sprite);

		/// <summary>
		/// 描画するスプライトを設定する
		/// この関数はindex:0のスプライトを設定する
		/// </summary>
		/// <param name="sprite">スプライト</param>
		void SetSprite(std::shared_ptr<class Sprite> sprite);

		/// <summary>
		/// 描画するスプライトを追加する
		/// 末尾に追加される
		/// </summary>
		/// <param name="sprite">スプライト</param>
		void AddSprite(class Sprite* sprite);

		/// <summary>
		/// 描画するスプライトを追加する
		/// 末尾に追加される
		/// </summary>
		/// <param name="sprite">スプライト</param>
		void AddSprite(std::shared_ptr<class Sprite> sprite);

		/// <summary>
		/// 描画するスプライトを指定されたインデックスのスプライトに変更する
		/// </summary>
		/// <param name="index">描画するスプライトのインデックス</param>
		void ChangeRenderSprite(size_t index);

		/// <summary>
		/// スプライトの描画モードを設定する
		/// </summary>
		/// <param name="drawMode"></param>
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
		/// <summary>
		/// シェーダーに渡す定数バッファのインデックス
		/// </summary>
		enum class CONSTANT_BUFFER_INDEX
		{
			TRANSFORM = 0,
			CAMERA = 1,
			DRAW_MODE = 2,
			MATERIAL = 3,
			BUFFER_COUNT
		};

		// スプライト描画で共通する部分のデータ
		std::shared_ptr<DX12Wrapper::RootSignature> m_rootSignature;
		std::shared_ptr<DX12Wrapper::GraphicsPipelineState> m_pipelineState;
		static unsigned int m_spriteCount;	// スプライトの数

		// スプライト固有のデータ
		std::shared_ptr<DX12Wrapper::ConstantBuffer> m_drawModeBuffer = nullptr;	// スプライトの描画モードをシェーダーに渡すためのバッファ
		std::vector<std::shared_ptr<class Sprite>> m_sprites;	                    // 描画するスプライト
		size_t m_currentSpriteIndex = 0;	                                        // 現在描画中のスプライトのインデックス
		UINT m_layer = 0;	                                                        // 描画順を制御するレイヤー
		SPRITE_DRAW_MODE m_drawMode = SPRITE_DRAW_MODE::GAMEOBJECT;	                // スプライトの描画モード
	};
}
