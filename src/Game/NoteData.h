#pragma once

namespace FlappyBird
{
	/// <summary>
	/// �m�[�c�̃f�[�^
	/// </summary>
	struct NoteData
	{
		float timing;
		float posY;

		NoteData() : timing(0.f), posY(0.f) {}
		NoteData(float timing, float posY) : timing(timing), posY(posY) {}
	};
}