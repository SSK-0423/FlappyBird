#pragma once
#include "NoteData.h"

namespace FlappyBird
{
	/// <summary>
	/// 譜面の情報を格納するクラス
	/// </summary>
	class FumenData
	{
	public:
		std::string musicName = "None";
		std::string musicFilePath = "None";
		float bpm = 120;
		int beat = 4;

		std::vector<NoteData> noteDatas;
		std::vector<HiddenNoteData> hiddenNoteDatas;
	};
}