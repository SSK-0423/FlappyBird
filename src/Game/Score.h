#pragma once

#include "Framework/GameFramework.h"
#include "JudgeResult.h"

namespace FlappyBird
{
	class GameMaster;

	/// <summary>
	/// スコアを管理するコンポーネント
	/// </summary>
	class Score : public Framework::IComponent
	{
	public:
		Score(std::shared_ptr<Framework::Object> owner);
		~Score() = default;

		void Start() override;
		void Update(float deltaTime) override;
		void Draw() override;

		void AddScore(unsigned int score);
		unsigned int GetScore();

	private:
		GameMaster* m_gameMaster;

		unsigned int m_score;
		float m_scoreAddInterval;
		float m_elapsedTime;

		void AddHiddenNoteScore(JUDGE_RESULT judgeResult);
	};
}