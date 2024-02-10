#pragma once

namespace FlappyBird
{
	class TimingCalculator
	{
	public:
		TimingCalculator() = default;
		~TimingCalculator() = default;

		static float CalcTiming(float judgeLineX, float targetPosX, float viewportWidth, float currentTime);
	};
}