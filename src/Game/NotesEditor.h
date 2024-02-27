#pragma once

#include "Framework/GameFramework.h"
#include "FumenData.h"

namespace FlappyBird
{
	class BarManager;
	class LaneManager;
	class NotesManager;
	class HiddenNotesManager;
	class MusicPlayer;

	class Obstacle;
	class JumpPoint;

	/// <summary>
	/// 譜面制作エディタ全体の流れを管理するクラス
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

		void PutNotes(NoteType type, float timing, float posY, float spaceOffset);
		void DeleteNotes(NoteType type, float timing);
		void Scroll(LONG mouseWheelMovement);

		/// <summary>
		/// ノーツのタイミングを計算する
		/// </summary>
		/// <param name="targetPosX">計算対象のX座標</param>
		/// <param name="viewportWidth">ビューポートの横幅</param>
		/// <returns>targetPosX座標に最も近い小節線のタイミング</returns>
		float CalcNotesTiming(LONG targetPosX, float viewportWidth);

		bool IsInsideViewport(POINT mousePos, CD3DX12_VIEWPORT viewport);

		BarManager* m_barManager = nullptr;
		LaneManager* m_laneManager = nullptr;
		NotesManager* m_notesManager = nullptr;
		HiddenNotesManager* m_hiddenNotesManager = nullptr;
		MusicPlayer* m_musicPlayer = nullptr;

		Obstacle* m_obstacle = nullptr;
		JumpPoint* m_jumpPoint = nullptr;

		Framework::SoundClip* m_putNotesSound = nullptr;
		Framework::SoundClip* m_deleteNotesSound = nullptr;
		Framework::SoundClip* m_cannotPutNotesSound = nullptr;

		NoteType m_putNoteType = NoteType::OBSTACLE;		// 設置するノーツの種類
		float m_judgeLineX;				                    // 判定ラインのX座標
		float m_spaceOffset = 0.0f;		                    // 土管の上下間のスペースを基準からずらす量
		bool m_isStartedEdit = false;	                    // 譜面制作が開始されているかどうか
	};
}