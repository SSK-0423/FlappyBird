#pragma once

#include "Framework/GameFramework.h"
#include "FumenData.h"

namespace FlappyBird
{
	class BarManager;
	class LaneManager;
	class NotesManager;
	class MusicPlayer;

	class Obstacle;

	/// <summary>
	/// ���ʐ���G�f�B�^�S�̗̂�����Ǘ�����N���X
	/// </summary>
	class NotesEditor : public Framework::IComponent
	{
	public:
		NotesEditor(std::shared_ptr<Framework::Object> owner);
		~NotesEditor();

		void Start();
		void Update(float deltaTime) override;
		void Draw() override;

	private:
		void SaveFumen(const std::string& savePath, FumenData& data);
		void LoadFumen(const std::string& loadPath, FumenData& data);
		void Play();
		void Stop();
		void Restart();
		void StartEdit(const FumenData& data);

		void PutNotes(float timing, float posY);
		void DeleteNotes(float timing, float posY);
		void Scroll(LONG mouseWheelMovement);

		/// <summary>
		/// �m�[�c�̃^�C�~���O���v�Z����
		/// </summary>
		/// <param name="targetPosX">�v�Z�Ώۂ�X���W</param>
		/// <param name="viewportWidth">�r���[�|�[�g�̉���</param>
		/// <returns>targetPosX���W�ɍł��߂����ߐ��̃^�C�~���O</returns>
		float CalcNotesTiming(LONG targetPosX, float viewportWidth);

		bool IsInsideViewport(POINT mousePos, CD3DX12_VIEWPORT viewport);

		BarManager* m_barManager = nullptr;
		LaneManager* m_laneManager = nullptr;
		NotesManager* m_notesManager = nullptr;
		MusicPlayer* m_musicPlayer = nullptr;

		Obstacle* m_obstacle = nullptr;

		Framework::SoundClip* m_putNotesSound = nullptr;
		Framework::SoundClip* m_deleteNotesSound = nullptr;
		Framework::SoundClip* m_cannotPutNotesSound = nullptr;

		float m_judgeLineX;				// ���胉�C����X���W
		bool m_isStartedEdit = false;	// ���ʐ��삪�J�n����Ă��邩�ǂ���
	};
}