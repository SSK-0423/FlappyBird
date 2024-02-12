#pragma once
#include "NoteData.h"

namespace FlappyBird
{
	/// <summary>
	/// •ˆ–Ê‚Ìî•ñ‚ğŠi”[‚·‚éƒNƒ‰ƒX
	/// </summary>
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