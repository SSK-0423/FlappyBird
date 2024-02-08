#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class MusicSelectSceneUI : public Framework::IComponent
	{
	public:
		MusicSelectSceneUI(std::shared_ptr<Framework::Object> owner);
		~MusicSelectSceneUI() = default;

		void Update(float deltaTime) override;
		void Draw() override;

	private:
		void LoadMusicList();
		void ChangeSelectedMusic();
	};
}