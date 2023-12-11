#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class ObstaclePool;
	class GameMaster;

	class ObstacleSpawner : public Framework::IComponent
	{
	public:
		ObstacleSpawner(Framework::Object* owner);
		~ObstacleSpawner();

		void Update(float deltaTime) override;
		void Draw() override;

	private:
		const unsigned int SPACE = 225;
		float m_spawnInverval = 2.0f;
		float m_spawnTimer = 0.0f;
		float m_speedUpTimer = 0.0f;
		float m_speedUpInterval = 10.0f;
		float m_obstacleSpeed = 150.0f;
		float m_maxObstacleSpeed = 300.0f;

		ObstaclePool* m_obstaclePool;
		GameMaster* m_gameMaster;

		std::mt19937_64 m_randomEngine;
		std::uniform_int_distribution<unsigned int> m_randomGenerator;

		void SpawnObstacle();
	};
}