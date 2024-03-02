#include "JudgeResult.h"

#include <SpriteFont.h>

namespace FlappyBird
{
	// ê√ìIÉÅÉìÉoïœêîÇÃé¿ëÃâª
	const float JudgeRange::PERFECT_JUDGE_RANGE = 66.67f;
	const float JudgeRange::GREAT_JUDGE_RANGE = 133.33f;
	const float JudgeRange::GOOD_JUDGE_RANGE = 200.0f;

	const int JudgeScore::PERFECT_SCORE = 100;
	const int JudgeScore::GREAT_SCORE = 50;
	const int JudgeScore::GOOD_SCORE = 10;

	const std::wstring JudgeText::PERFECT_TEXT = L"PERFECT ! ! !";
	const std::wstring JudgeText::GREAT_TEXT = L"GREAT ! !";
	const std::wstring JudgeText::GOOD_TEXT = L"GOOD !";

	const DirectX::XMVECTORF32 JudgeColor::PERFECT_COLOR = { 0.0f, 1.0f, 0.0f, 1.0f };
	const DirectX::XMVECTORF32 JudgeColor::GREAT_COLOR = { 1.0f, 1.0f, 0.0f, 1.0f };
	const DirectX::XMVECTORF32 JudgeColor::GOOD_COLOR = { 1.0f, 0.0f, 0.0f, 1.0f };
}