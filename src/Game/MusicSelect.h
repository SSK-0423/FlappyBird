#pragma once

#include "Framework/GameFramework.h"
#include "FumenData.h"

namespace FlappyBird
{
	class MusicSelect : public Framework::IComponent
	{
	public:
		MusicSelect(std::shared_ptr<Framework::Object> owner);
		~MusicSelect();

		void Start() override;
		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		static FumenData selectedFumenData;
	};
}