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

		void PutNotes(float timing, float posY);
		void DeleteNotes(float timing, float posY);
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
		MusicPlayer* m_musicPlayer = nullptr;

		Obstacle* m_obstacle = nullptr;

		Framework::SoundClip* m_putNotesSound = nullptr;
		Framework::SoundClip* m_deleteNotesSound = nullptr;
		Framework::SoundClip* m_cannotPutNotesSound = nullptr;

		float m_judgeLineX;				// 判定ラインのX座標
		bool m_isStartedEdit = false;	// 譜面制作が開始されているかどうか
	};
}