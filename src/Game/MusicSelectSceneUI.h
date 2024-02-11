#pragma once

#include "Framework/GameFramework.h"
#include "FumenData.h"

namespace FlappyBird
{
	class MusicDataFrame;

	class MusicSelectSceneUI : public Framework::IComponent
	{
	public:
		MusicSelectSceneUI(std::shared_ptr<Framework::Object> owner);
		~MusicSelectSceneUI() = default;

		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		Framework::Subject<FumenData&> OnChangeSelectedMusic;
	private:
		std::vector<FumenData> m_fumenDatas;
		std::vector<std::string> m_fumenPaths;

		std::vector<MusicDataFrame*> m_musicDataFrames;

		void LoadMusicList();
		void CreateMusicDataFrames();
		void ChangeSelectedMusic(bool isNext);
		void UpdateMusicDataFramesActive();

		FumenData m_selectedFumenData;
		int m_selectedMusicIndex;
	};
}