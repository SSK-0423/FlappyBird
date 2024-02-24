#pragma once

namespace FlappyBird
{
	/// <summary>
	/// ノーツのデータ
	/// </summary>
	struct NoteData
	{
		float timing;
		float posY;
		float spaceOffset;

		NoteData() : timing(0.f), posY(0.f), spaceOffset(0.f) {}
		NoteData(float timing, float posY, float spaceOffset) : timing(timing), posY(posY), spaceOffset(spaceOffset) {}
	};
}