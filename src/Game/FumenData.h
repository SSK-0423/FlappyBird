#pragma once
#include "NoteData.h"

namespace FlappyBird
{
	class FumenData
	{
	public:
		std::string musicName = "None";
		std::string musicFilePath = "None";
		float bpm = 120;
		int beat = 4;

		std::vector<NoteData> noteDatas;
	};
}