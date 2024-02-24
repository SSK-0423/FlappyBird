#pragma once
#include "Framework/GameFramework.h"
#include "NoteData.h"

namespace FlappyBird
{
	class Obstacle;
	class MusicPlayer;

	/// <summary>
	/// �m�[�c���Ǘ�����N���X
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
		/// ���ʃt�@�C������ǂݍ��񂾃m�[�c���Z�b�g����
		/// </summary>
		/// <param name="notes"></param>
		void SetNotes(const std::vector<NoteData>& notes);

		/// <summary>
		/// �S�m�[�c��SE�Đ��\�ɂ���
		/// </summary>
		void ResetCanPlaySE();

		/// <summary>
		/// �S�m�[�c���폜����
		/// </summary>
		void DeleteAllNotes();

		Framework::Subject<Framework::NotificationEvent> OnReachedJudgeLine;	// �m�[�c�����胉�C���ɓ��B�����Ƃ��̃C�x���g

	private:
		std::vector<NoteData> m_notes;	        // �f�[�^�݂̂̃m�[�c���X�g
		std::vector<Obstacle*> m_noteObstacles;	// �m�[�c�̕`��I�u�W�F�N�g���X�g

		MusicPlayer* m_musicPlayer = nullptr;

		float m_currentPlayTime = 0.0f;	        // ���݂̍Đ�����
		float m_judgeLineX = 0.0f;	            // ���胉�C����X���W

		const float TIMING_OFFSET = 100.0f;	    // �m�[�c�̃A�N�e�B�u����̃^�C�~���O�I�t�Z�b�g
		const float PLAY_SE_TIMING_RANGE = 16.67f;	// �m�[�c��SE�Đ��̃^�C�~���O�͈�

		/// <summary>
		/// MusicPlayer�̍Đ����Ԃ��擾���AObstacle�̍Đ����Ԃɔ��f������
		/// </summary>
		void UpdateCurrentPlayTime();

		/// <summary>
		/// �m�[�c�̃A�N�e�B�u��Ԃ��X�V����
		/// </summary>
		void UpdateNoteActive();

		/// <summary>
		/// �m�[�c��SE���Đ�����
		/// </summary>
		void PlayNoteSound();
	};
}