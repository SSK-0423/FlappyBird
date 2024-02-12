#pragma once
#include "Framework/GameFramework.h"

namespace FlappyBird
{
	/// <summary>
	/// PleaseClickTextを点滅させながら表示するコンポーネント
	/// </summary>
	class PleaseClickText : public Framework::IComponent
	{
	public:
		PleaseClickText(std::shared_ptr<Framework::Object> owner);
		~PleaseClickText() = default;

		void Update(float deltaTime) override;
		void Draw() override;

		Framework::Subject<Framework::NotificationEvent> OnClicked; // クリックされたときに通知するイベント

	private:
		float m_elapsedBlinkTime;		// 点滅アニメーションの経過時間
		float m_blinkAnimationInterval;	// 点滅の間隔

		float m_elapsedWaitTime;		// 決定音の再生終了までの経過時間
		float m_waitSoundTime;			// 決定音の再生時間

		bool m_isClicked;				// クリックされたかどうか

		std::vector<DirectX::XMVECTORF32> m_colors;	// 黒と透明の色
		size_t m_currentColorIndex;					// 現在の色のインデックス

		/// <summary>
		/// 点滅アニメーション
		/// </summary>
		/// <param name="deltaTime"></param>
		void BlinkAnimation(float deltaTime);
	};
}