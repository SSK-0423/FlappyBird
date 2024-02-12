#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class MusicPlayer;

	/// <summary>
	/// �B���m�[�c���Ǘ�����R���|�[�l���g
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
		/// �B���m�[�c�𐶐�����
		/// </summary>
		/// <param name="barNum">���ߐ�</param>
		/// <param name="bpm">BPM</param>
		/// <param name="beat">���q</param>
		void CreateHiddenNotes(unsigned int barNum, float bpm, float beat);
	
	private:
		/// <summary>
		/// �B���m�[�c�̃f�[�^
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

		size_t m_currentHiddenNoteIndex = 0;	// ���胉�C���ɍł��߂��B���m�[�c�̃C���f�b�N�X

		const float PERFECT_JUDGE_RANGE;	// �p�[�t�F�N�g����͈̔�

		/// <summary>
		/// �B���m�[�c�̃A�N�e�B�u��Ԃ��X�V����
		/// </summary>
		void UpdateHiddenNotesState();

		/// <summary>
		/// �B���m�[�c�Ƃ̃^�C�~���O�𔻒肷��
		/// </summary>
		void JudgeTiming();
	};
}