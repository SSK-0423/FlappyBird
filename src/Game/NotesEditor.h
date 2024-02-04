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

	class NotesEditor : public Framework::IComponent
	{
	public:
		NotesEditor(Framework::Object* owner);
		~NotesEditor();

		void Start();
		void Update(float deltaTime) override;
		void Draw() override;

	private:
		void SaveFumen(const std::string& savePath, const FumenData& data);
		void LoadFumen(const std::string& loadPath, FumenData& data);
		void Play();
		void Stop();
		void LoadMusic(const std::string& musicPath);
		void StartEdit(const FumenData& data);

		void PutNotes(float timing, float posY);
		void DeleteNotes(float timing, float posY);

		float CalcNotesTiming(LONG mouseX, float viewportWidth);
		bool IsInsideViewport(POINT mousePos, CD3DX12_VIEWPORT viewport);

		BarManager* m_barManager = nullptr;
		LaneManager* m_laneManager = nullptr;
		NotesManager* m_notesManager = nullptr;
		MusicPlayer* m_musicPlayer = nullptr;

		Obstacle* m_obstacle = nullptr;
	};
}