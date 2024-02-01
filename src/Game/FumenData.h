#pragma once

namespace FlappyBird
{
	class FumenData
	{
	public:
		std::string musicName = "None";
		std::string musicFilePath = "None";
		float bpm = 120;
		int beat = 4;

		std::vector<float> notesTiming;
	};
}