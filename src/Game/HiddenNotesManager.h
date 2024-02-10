#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class MusicPlayer;

	class HiddenNotesManager : public Framework::IComponent
	{
	public:
		HiddenNotesManager(std::shared_ptr<Framework::Object> owner);
		~HiddenNotesManager() = default;

		void Start();
		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		void CreateHiddenNotes(unsigned int barNum, float bpm, float beat);
	private:
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

		size_t m_currentHiddenNoteIndex = 0;

		const float PERFECT_JUDGE_RANGE;

		void UpdateHiddenNotesState();
		void JudgeTiming();
	};
}