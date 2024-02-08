#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class GameMaster;

	class Score : public Framework::IComponent
	{
	public:
		Score(std::shared_ptr<Framework::Object> owner);
		~Score() = default;

		void Update(float deltaTime) override;
		void Draw() override;

		void AddScore(unsigned int score);
		unsigned int GetScore();

	private:
		GameMaster* m_gameMaster;

		unsigned int m_score;
		float m_scoreAddInterval;
		float m_elapsedTime;
	};
}