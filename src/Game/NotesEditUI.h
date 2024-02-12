#pragma once

#include "../Framework/GameFramework.h"
#include "FumenData.h"

namespace FlappyBird
{
	/// <summary>
	/// ノーツエディット画面のUIを管理するコンポーネントs
	/// </summary>
	class NotesEditUI : public Framework::IComponent
	{
	public:
		NotesEditUI(std::shared_ptr<Framework::Object> owner);
		~NotesEditUI() = default;

		void Update(float deltaTime) override;
		void Draw() override;

		// 各ボタン押下時のイベント
		Framework::Subject<const std::string&, FumenData&> OnSave;	// Saveボタン押下時
		Framework::Subject<const std::string&, FumenData&> OnLoad;	// Loadボタン押下時
		Framework::Subject<const FumenData&> OnEditStart;			// EditStartボタン押下時
		Framework::Subject<Framework::NotificationEvent> OnPlay;	// Playボタン押下時
		Framework::Subject<Framework::NotificationEvent> OnStop;	// Stopボタン押下時
		Framework::Subject<Framework::NotificationEvent> OnRestart;	// Restartボタン押下時

	private:
		static const ImVec2 WINDOW_POS;
		static const ImVec2 WINDOW_SIZE;

		FumenData m_fumenData;

		/// <summary>
		/// 譜面データを描画する
		/// ファイル名やファイルパス, BPM, 拍子など
		/// ノーツを描画するわけではない
		/// </summary>
		void DrawFumenData();

		/// <summary>
		/// セーブ・ロードボタンを描画する
		/// </summary>
		void DrawSaveLoadButton();

		/// <summary>
		/// 再生・停止・リスタートボタンを描画する
		/// </summary>
		void DrawPlayStopRestartButton();

		/// <summary>
		/// エディット開始ボタンを描画する
		/// </summary>
		void DrawEditStartButton();

		static std::string m_selectedMusicPath;		// 選択中の曲のファイルパス
	};
}