#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class MusicDataFrame : public Framework::IComponent
	{
	public:
		MusicDataFrame(std::shared_ptr<Framework::Object> owner);
		~MusicDataFrame() = default;

		void Update(float deltaTime) override;
		void Draw() override;

		void FitTextPosition();
	};
}