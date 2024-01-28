#pragma once

namespace FlappyBird
{
	class FumenData
	{
	public:
		char musicName[256] = "None";
		std::string musicFilePath = "None";
		float bpm = 120;
		int beat = 4;
	};
}