#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class BarLine : public Framework::IComponent
	{
	public:
		BarLine(Framework::Object* owner);
		~BarLine();

		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		void SetTiming(float timing);

	private:
		float m_timing = 0.0f;
	};
}