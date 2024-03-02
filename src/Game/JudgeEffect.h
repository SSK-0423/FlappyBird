#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class JudgeEffect : public Framework::IComponent
	{
	public:
		JudgeEffect(std::shared_ptr<Framework::Object> owner);
		~JudgeEffect();

		void Update(float deltaTime) override;
		void Draw() override;

		void SetJudgeText(const std::wstring& text);
		void SetJudgeTextColor(const DirectX::XMVECTORF32& color);

	private:
		static float m_riseSpeed;
		static float m_lifeTime;
		static float m_initPosY;

		Framework::Text* m_judgeText;
		Framework::Transform2D* m_transform;

		float m_timer = 0.0f;

		/// <summary>
		/// エフェクトの寿命をチェックして寿命がくれば非アクティブにする
		/// </summary>
		void CheckLifeTime();

		/// <summary>
		/// テキストの上昇アニメーション
		/// </summary>
		void RiseAnimation();
	};
}