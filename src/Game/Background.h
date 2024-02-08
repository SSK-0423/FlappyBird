#pragma once

#include "Framework/IComponent.h"

namespace FlappyBird
{
	class Background : public Framework::IComponent
	{
	public:
		Background(std::shared_ptr<Framework::Object> owner);
		~Background();

		void Update(float deltaTime) override;
		void Draw() override;
	};
}