#pragma once

namespace FlappyBird
{
	enum class NoteType
	{
		OBSTACLE,	// ��Q��(�y��)�̔z�u��\���m�[�c
		HIDDEN,		// �B���m�[�c
	};

	/// <summary>
	/// ��Q��(�y��)�̔z�u�������m�[�c�̃f�[�^
	/// </summary>
	struct NoteData
	{
		NoteType type;
		float timing;
		float posY;
		float spaceOffset;

		NoteData() : type(NoteType::OBSTACLE), timing(0.f), posY(0.f), spaceOffset(0.f) {}
		NoteData(float timing, float posY, float spaceOffset)
			: type(NoteType::OBSTACLE), timing(timing), posY(posY), spaceOffset(spaceOffset) {}
	};

	/// <summary>
	/// �B���m�[�c�̃f�[�^
	/// </summary>
	struct HiddenNoteData
	{
		NoteType type;
		float timing;
		bool isJudged;

		HiddenNoteData() : type(NoteType::HIDDEN), timing(0.f), isJudged(false) {}
		HiddenNoteData(float timing) : type(NoteType::HIDDEN), timing(timing), isJudged(false) {}
	};
}