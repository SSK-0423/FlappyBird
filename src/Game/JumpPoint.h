#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class JumpPoint : public Framework::IComponent
	{
	public:
		JumpPoint(std::shared_ptr<Framework::Object> owner);
		~JumpPoint();

		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		void SetTiming(float timing);
		float GetTiming();

		void Reset();

		static void SetJudgeLineX(float judgeLineX);
		static void SetCurrentPlayTime(float currentPlayTime);

	private:
		float m_timing = 0.0f;

		Framework::Transform2D* m_transform = nullptr;

		void UpdatePosition();

		static float m_judgeLineX;
		static float m_currentPlayTime;
		static UINT m_jumpPointCount;
	};
}