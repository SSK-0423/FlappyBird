#pragma once

#include "Framework/GameFramework.h"

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

		static std::string selectedMusicPath;
	private:
	};
}