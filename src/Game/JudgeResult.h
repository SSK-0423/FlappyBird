#pragma once
#include <DirectXMath.h>

namespace FlappyBird
{
	/// <summary>
	/// ���茋��
	/// </summary>
	enum class JUDGE_RESULT
	{
		PERFECT,
		GREAT,
		GOOD,
	};

	struct JudgeRange
	{
		static const float PERFECT_JUDGE_RANGE;		// �p�[�t�F�N�g����͈̔�
		static const float GREAT_JUDGE_RANGE;		// �O���[�g����͈̔�
		static const float GOOD_JUDGE_RANGE;		// �O�b�h����͈̔�
	};

	struct JudgeScore
	{
		static const int PERFECT_SCORE;		// �p�[�t�F�N�g����̃X�R�A
		static const int GREAT_SCORE;		// �O���[�g����̃X�R�A
		static const int GOOD_SCORE;		// �O�b�h����̃X�R�A
	};

	struct JudgeText
	{
		static const std::wstring PERFECT_TEXT;		// �p�[�t�F�N�g����̃e�L�X�g
		static const std::wstring GREAT_TEXT;		// �O���[�g����̃e�L�X�g
		static const std::wstring GOOD_TEXT;		// �O�b�h����̃e�L�X�g
	};

	struct JudgeColor
	{
		static const DirectX::XMVECTORF32 PERFECT_COLOR;	// �p�[�t�F�N�g����̐F
		static const DirectX::XMVECTORF32 GREAT_COLOR;		// �O���[�g����̐F
		static const DirectX::XMVECTORF32 GOOD_COLOR;		// �O�b�h����̐F
	};
}