#pragma once
#include "Framework/GameFramework.h"
#include "NoteData.h"

namespace FlappyBird
{
	class Obstacle;
	class MusicPlayer;

	/// <summary>
	/// ノーツを管理するクラス
	/// </summary>
	class NotesManager : public Framework::IComponent
	{
	public:
		NotesManager(std::shared_ptr<Framework::Object> owner);
		~NotesManager();

		void Start();
		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		bool CreateNotes(NoteData data);
		bool DeleteNotes(float timing);

		std::vector<NoteData>& GetNotes();

		/// <summary>
		/// 譜面ファイルから読み込んだノーツをセットする
		/// </summary>
		/// <param name="notes"></param>
		void SetNotes(const std::vector<NoteData>& notes);

		/// <summary>
		/// 全ノーツをSE再生可能にする
		/// </summary>
		void ResetCanPlaySE();

		/// <summary>
		/// 全ノーツを削除する
		/// </summary>
		void DeleteAllNotes();

		Framework::Subject<Framework::NotificationEvent> OnReachedJudgeLine;	// ノーツが判定ラインに到達したときのイベント

	private:
		std::vector<NoteData> m_notes;	        // データのみのノーツリスト
		std::vector<Obstacle*> m_noteObstacles;	// ノーツの描画オブジェクトリスト

		MusicPlayer* m_musicPlayer = nullptr;

		float m_currentPlayTime = 0.0f;	        // 現在の再生時間
		float m_judgeLineX = 0.0f;	            // 判定ラインのX座標

		const float TIMING_OFFSET = 100.0f;	    // ノーツのアクティブ判定のタイミングオフセット
		const float PLAY_SE_TIMING_RANGE = 16.67f;	// ノーツのSE再生のタイミング範囲

		/// <summary>
		/// MusicPlayerの再生時間を取得し、Obstacleの再生時間に反映させる
		/// </summary>
		void UpdateCurrentPlayTime();

		/// <summary>
		/// ノーツのアクティブ状態を更新する
		/// </summary>
		void UpdateNoteActive();

		/// <summary>
		/// ノーツのSEを再生する
		/// </summary>
		void PlayNoteSound();
	};
}