#pragma once

#include "../Framework/GameFramework.h"
#include "FumenData.h"

namespace FlappyBird
{
	/// <summary>
	/// �m�[�c�G�f�B�b�g��ʂ�UI���Ǘ�����R���|�[�l���gs
	/// </summary>
	class NotesEditUI : public Framework::IComponent
	{
	public:
		NotesEditUI(std::shared_ptr<Framework::Object> owner);
		~NotesEditUI() = default;

		void Update(float deltaTime) override;
		void Draw() override;

		// �e�{�^���������̃C�x���g
		Framework::Subject<const std::string&, FumenData&> OnSave;	// Save�{�^��������
		Framework::Subject<const std::string&, FumenData&> OnLoad;	// Load�{�^��������
		Framework::Subject<const FumenData&> OnEditStart;			// EditStart�{�^��������
		Framework::Subject<Framework::NotificationEvent> OnPlay;	// Play�{�^��������
		Framework::Subject<Framework::NotificationEvent> OnStop;	// Stop�{�^��������
		Framework::Subject<Framework::NotificationEvent> OnRestart;	// Restart�{�^��������

	private:
		static const ImVec2 WINDOW_POS;
		static const ImVec2 WINDOW_SIZE;

		FumenData m_fumenData;

		/// <summary>
		/// ���ʃf�[�^��`�悷��
		/// �t�@�C������t�@�C���p�X, BPM, ���q�Ȃ�
		/// �m�[�c��`�悷��킯�ł͂Ȃ�
		/// </summary>
		void DrawFumenData();

		/// <summary>
		/// �Z�[�u�E���[�h�{�^����`�悷��
		/// </summary>
		void DrawSaveLoadButton();

		/// <summary>
		/// �Đ��E��~�E���X�^�[�g�{�^����`�悷��
		/// </summary>
		void DrawPlayStopRestartButton();

		/// <summary>
		/// �G�f�B�b�g�J�n�{�^����`�悷��
		/// </summary>
		void DrawEditStartButton();

		static std::string m_selectedMusicPath;		// �I�𒆂̋Ȃ̃t�@�C���p�X
	};
}