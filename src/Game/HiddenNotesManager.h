#pragma once

#include "Framework/GameFramework.h"
#include "NoteData.h"

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

		/// <summary>
		/// 隠しノーツを生成する
		/// </summary>
		/// <param name="barNum">小節数</param>
		/// <param name="bpm">BPM</param>
		/// <param name="beat">拍子</param>
		void CreateHiddenNotes(unsigned int barNum, float bpm, float beat);

		bool CreateHiddenNotes(HiddenNoteData data);
		bool DeleteHiddenNotes(HiddenNoteData data);

		void SetHiddenNotes(const std::vector<HiddenNoteData>& hiddenNotes);

		std::vector<HiddenNoteData>& GetHiddenNotes();

	private:
		std::vector<HiddenNoteData> m_hiddenNotes;
		std::vector<JumpPoint*> m_jumpPoints;

		Framework::SoundClip* m_seClip = nullptr;
		MusicPlayer* m_musicPlayer = nullptr;

		float m_judgeLineX = 0.0f;	            // 判定ラインのX座標

		size_t m_currentHiddenNoteIndex = 0;	// 判定ラインに最も近い隠しノーツのインデックス

		const float PERFECT_JUDGE_RANGE;		// パーフェクト判定の範囲
		const float TIMING_OFFSET = 100.0f;	    // ノーツのアクティブ判定のタイミングオフセット

		/// <summary>
		/// MusicPlayerの再生時間を取得し、JumpPointの再生時間に反映させる
		/// </summary>		
		void UpdateCurrentPlayTime();

		void UpdateHiddenNotesActive();

		/// <summary>
		/// 隠しノーツのアクティブ状態を更新する
		/// </summary>
		void UpdateHiddenNotesState();

		/// <summary>
		/// 隠しノーツとのタイミングを判定する
		/// </summary>
		void JudgeTiming();
	};
}