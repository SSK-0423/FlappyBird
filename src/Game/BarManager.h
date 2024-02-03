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

		void CreateBarLine();
		void Update(float deltaTime) override;
		void Draw() override;


	private:
		// 小節線の一覧
		std::vector<BarLine> m_barLines;
	};
}