#pragma once

#include "Framework/GameFramework.h"

#include "Player.h"

namespace FlappyBird
{
	class Score : public Framework::IComponent
	{
	public:
		Score(Framework::Object* owner);
		~Score() = default;

		void Update(float deltaTime) override;
		void Draw() override;

		void AddScore(unsigned int score);
		unsigned int GetScore();

	private:
		unsigned int m_score;
		float m_scoreAddInterval;
		float m_elapsedTime;

		Player* m_player;
	};
}