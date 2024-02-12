#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	/// <summary>
	/// 小節線の種類
	/// </summary>
	enum class BARLINETYPE
	{
		WHOLE,
		QUARTER,
		EIGHTH,
	};

	/// <summary>
	/// 小節線のコンポーネント
	/// </summary>
	class BarLine : public Framework::IComponent
	{
	public:
		BarLine(std::shared_ptr<Framework::Object> owner);
		~BarLine();

		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		void SetTiming(float timing);
		float GetTiming();

		bool CanPlaySE();
		void SetCanPlaySE(bool canPlaySE);

		void SetBarLineType(BARLINETYPE type);

		static void SetCurrentPlayTime(float currentPlayTime);
		static void SetJudgeLineX(float judgeLineX);
	private:
		Framework::Transform2D* m_transform = nullptr;
		float m_timing = 0.0f;	        // この小節線のタイミング
		bool m_canPlaySE = true;

		void UpdatePosition();

		static float m_judgeLineX;	    // 判定ラインのX座標
		static float m_currentPlayTime;	// 現在の再生時間
	};
}