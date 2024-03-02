#pragma once
#include <DirectXMath.h>

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

	struct JudgeScore
	{
		static const int PERFECT_SCORE;		// パーフェクト判定のスコア
		static const int GREAT_SCORE;		// グレート判定のスコア
		static const int GOOD_SCORE;		// グッド判定のスコア
	};

	struct JudgeText
	{
		static const std::wstring PERFECT_TEXT;		// パーフェクト判定のテキスト
		static const std::wstring GREAT_TEXT;		// グレート判定のテキスト
		static const std::wstring GOOD_TEXT;		// グッド判定のテキスト
	};

	struct JudgeColor
	{
		static const DirectX::XMVECTORF32 PERFECT_COLOR;	// パーフェクト判定の色
		static const DirectX::XMVECTORF32 GREAT_COLOR;		// グレート判定の色
		static const DirectX::XMVECTORF32 GOOD_COLOR;		// グッド判定の色
	};
}