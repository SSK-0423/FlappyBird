#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
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
	
	private:
		/// <summary>
		/// 隠しノーツのデータ
		/// </summary>
		struct HiddenNote
		{
			float timing;
			bool isJudged;

			HiddenNote() :timing(0.f), isJudged(false) {};
			HiddenNote(float timing, bool isJudged)
				: timing(timing), isJudged(isJudged)
			{}
		};
		std::vector<HiddenNote> m_hiddenNotes;

		Framework::SoundClip* m_seClip = nullptr;
		MusicPlayer* m_musicPlayer = nullptr;

		size_t m_currentHiddenNoteIndex = 0;	// 判定ラインに最も近い隠しノーツのインデックス

		const float PERFECT_JUDGE_RANGE;	// パーフェクト判定の範囲

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