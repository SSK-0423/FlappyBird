#include "pch.h"
#include "TimingCalculator.h"

namespace FlappyBird
{
	float TimingCalculator::CalcTiming(float judgeLineX, float targetPosX, float viewportWidth, float currentTime)
	{
		float distanceX = viewportWidth - judgeLineX;
		float posX = (targetPosX - judgeLineX) * 2.f;
		float timing = posX / distanceX * 1000.f + currentTime;

		return timing;
	}
}