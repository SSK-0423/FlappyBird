#pragma once

#include "Framework/GameFramework.h"
#include "NoteData.h"
#include "JudgeResult.h"

namespace FlappyBird
{
	class JumpPoint;
	class MusicPlayer;

	/// <summary>
	/// 隠しノーツを管理するコンポーネント
	/// </summary>
	class HiddenNotesManager : public Framework::IComponent
	{
	public:
		HiddenNotesManager(std::shared_ptr<Framework::Object> owner);
		~HiddenNotesManager() = default;

		void Start();
		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		bool CreateHiddenNotes(HiddenNoteData data);
		bool DeleteHiddenNotes(HiddenNoteData data);

		void SetHiddenNotes(const std::vector<HiddenNoteData>& hiddenNotes);

		std::vector<HiddenNoteData>& GetHiddenNotes();

		Framework::Subject<Framework::NotificationEvent> OnReachedJudgeLine;	// ノーツが判定ラインに到達したときのイベント
		Framework::Subject<JUDGE_RESULT> OnJudge;								// ノーツの判定時のイベント
	private:
		std::vector<HiddenNoteData> m_hiddenNotes;
		std::vector<JumpPoint*> m_jumpPoints;

		Framework::SoundClip* m_seClip = nullptr;
		MusicPlayer* m_musicPlayer = nullptr;

		float m_judgeLineX = 0.0f;	            // 判定ラインのX座標

		size_t m_currentHiddenNoteIndex = 0;	// 判定ラインに最も近い隠しノーツのインデックス

		const float TIMING_OFFSET = 100.0f;	    // ノーツのアクティブ判定のタイミングオフセット

		/// <summary>
		/// MusicPlayerの再生時間を取得し、JumpPointの再生時間に反映させる
		/// </summary>		
		void UpdateCurrentPlayTime();

		/// <summary>
		/// 隠しノーツの判定状況を更新する
		/// </summary>
		void UpdateHiddenNotesState();

		/// <summary>
		/// 隠しノーツとのタイミングを判定する
		/// </summary>
		void JudgeTiming();

		/// <summary>
		/// 隠しノーツ判定時の処理
		/// </summary>
		/// <param name="result"></param>
		void JudgeEvent(JUDGE_RESULT result);

		/// <summary>
		/// 隠しノーツのアクティブ状態を更新する
		/// </summary>
		void UpdateHiddenNotesActive();
	};
}