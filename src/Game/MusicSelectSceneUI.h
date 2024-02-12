#pragma once

#include "Framework/GameFramework.h"
#include "FumenData.h"

namespace FlappyBird
{
	class MusicDataFrame;

	/// <summary>
	/// 曲選択シーンのUIを管理するコンポーネント
	/// </summary>
	class MusicSelectSceneUI : public Framework::IComponent
	{
	public:
		MusicSelectSceneUI(std::shared_ptr<Framework::Object> owner);
		~MusicSelectSceneUI() = default;

		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		Framework::Subject<FumenData&> OnChangeSelectedMusic;	// 曲が選択されたときに通知されるイベント
	private:
		std::vector<FumenData> m_fumenDatas;	                // 譜面データ
		std::vector<std::string> m_fumenPaths;	                // 譜面データのファイルパス

		std::vector<MusicDataFrame*> m_musicDataFrames;	        // 曲データ表示のUI

		Framework::SoundClip* m_changeMusicSound;	            // 曲変更時の効果音

		/// <summary>
		/// 曲(譜面)リストを読み込む
		/// </summary>
		void LoadMusicList();

		/// <summary>
		/// 曲データ表示のUIを作成する
		/// </summary>
		void CreateMusicDataFrames();

		/// <summary>
		/// 曲変更ボタンを作成する
		/// </summary>
		void CreateMusicChangeButtons();

		/// <summary>
		/// 曲を変更する
		/// </summary>
		/// <param name="isNext">次のインデックスかどうか</param>
		void ChangeSelectedMusic(bool isNext);

		/// <summary>
		/// 曲データ表示UIのアクティブ状態を更新する
		/// </summary>
		void UpdateMusicDataFramesActive();

		FumenData m_selectedFumenData;	// 選択中の譜面データ
		int m_selectedMusicIndex;	    // 選択中の曲のインデックス
	};
}