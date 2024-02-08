#pragma once

namespace FlappyBird
{
	struct NoteData
	{
		float timing;
		float posY;

		NoteData() : timing(0.f), posY(0.f) {}
		NoteData(float timing, float posY) : timing(timing), posY(posY) {}
	};
}