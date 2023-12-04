#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class ObstaclePool;

	class ObstacleSpawner : public Framework::IComponent
	{
	public:
		ObstacleSpawner(Framework::Object* owner);
		~ObstacleSpawner();

		void Update(float deltaTime) override;
		void Draw() override;

	private:
		const unsigned int SPACE = 200;
		float m_spawnInverval = 2.0f;
		float m_spawnTimer = 0.0f;

		ObstaclePool* m_obstaclePool;

		std::mt19937_64 m_randomEngine;
		std::uniform_int_distribution<unsigned int> m_randomGenerator;

		void SpawnObstacle();
	};
}