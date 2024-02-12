#pragma once

namespace FlappyBird
{
	/// <summary>
	/// タイミングを計算するクラス
	/// </summary>
	class TimingCalculator
	{
	public:
		TimingCalculator() = default;
		~TimingCalculator() = default;

		/// <summary>
		/// タイミングをミリ秒単位で計算する
		/// </summary>
		/// <param name="judgeLineX">判定ラインのX座標</param>
		/// <param name="targetPosX">タイミングを算出する対象のX座標</param>
		/// <param name="viewportWidth">ビューポートの横幅</param>
		/// <param name="currentTime">現在の再生時間</param>
		/// <returns>targetPosXにおけるタイミング(ミリ秒)</returns>
		static float CalcTiming(float judgeLineX, float targetPosX, float viewportWidth, float currentTime);
	};
}