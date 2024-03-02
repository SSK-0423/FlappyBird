#pragma once

namespace FlappyBird
{
	/// <summary>
	/// 判定結果
	/// </summary>
	enum class JUDGE_RESULT
	{
		PERFECT,
		GREAT,
		GOOD,
	};

	struct JudgeRange
	{
		static const float PERFECT_JUDGE_RANGE;		// パーフェクト判定の範囲
		static const float GREAT_JUDGE_RANGE;		// グレート判定の範囲
		static const float GOOD_JUDGE_RANGE;		// グッド判定の範囲
	};
}