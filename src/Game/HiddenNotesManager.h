#pragma once

#include "Framework/GameFramework.h"
#include "NoteData.h"
#include "JudgeResult.h"

namespace FlappyBird
{
	class JumpPoint;
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

		bool CreateHiddenNotes(HiddenNoteData data);
		bool DeleteHiddenNotes(HiddenNoteData data);

		void SetHiddenNotes(const std::vector<HiddenNoteData>& hiddenNotes);

		std::vector<HiddenNoteData>& GetHiddenNotes();

		Framework::Subject<Framework::NotificationEvent> OnReachedJudgeLine;	// �m�[�c�����胉�C���ɓ��B�����Ƃ��̃C�x���g
		Framework::Subject<JUDGE_RESULT> OnJudge;								// �m�[�c�̔��莞�̃C�x���g
	private:
		std::vector<HiddenNoteData> m_hiddenNotes;
		std::vector<JumpPoint*> m_jumpPoints;

		Framework::SoundClip* m_seClip = nullptr;
		MusicPlayer* m_musicPlayer = nullptr;

		float m_judgeLineX = 0.0f;	            // ���胉�C����X���W

		size_t m_currentHiddenNoteIndex = 0;	// ���胉�C���ɍł��߂��B���m�[�c�̃C���f�b�N�X

		const float TIMING_OFFSET = 100.0f;	    // �m�[�c�̃A�N�e�B�u����̃^�C�~���O�I�t�Z�b�g

		/// <summary>
		/// MusicPlayer�̍Đ����Ԃ��擾���AJumpPoint�̍Đ����Ԃɔ��f������
		/// </summary>		
		void UpdateCurrentPlayTime();

		/// <summary>
		/// �B���m�[�c�̔���󋵂��X�V����
		/// </summary>
		void UpdateHiddenNotesState();

		/// <summary>
		/// �B���m�[�c�Ƃ̃^�C�~���O�𔻒肷��
		/// </summary>
		void JudgeTiming();

		/// <summary>
		/// �B���m�[�c���莞�̏���
		/// </summary>
		/// <param name="result"></param>
		void JudgeEvent(JUDGE_RESULT result);

		/// <summary>
		/// �B���m�[�c�̃A�N�e�B�u��Ԃ��X�V����
		/// </summary>
		void UpdateHiddenNotesActive();
	};
}