#pragma once
#include "Framework/GameFramework.h"
#include "BarLine.h"

namespace FlappyBird
{
	class BarManager : public Framework::IComponent
	{
	public:
		BarManager(Framework::Object* owner);
		~BarManager() = default;

		void Update(float deltaTime) override;
		void Draw() override;

	private:
		// ¬ßü‚Ìˆê——
		std::vector<BarLine> m_barLines;
	};
}