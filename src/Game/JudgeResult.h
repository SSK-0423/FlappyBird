#pragma once

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
}