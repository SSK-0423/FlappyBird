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
		const int SPACE = 225;
		int m_lastSpawnY = 0;

		float m_spawnInverval = 2.0f;
		float m_spawnTimer = 0.0f;

		float m_spawnSpeedUpTimer = 0.0f;
		float m_spawnSpeedUpInterval = 10.0f;
		float m_spawnSpeedUpDiff = 0.1f;
		unsigned int m_spawnSpeedUpStep = 5;	// �ŏ������Ԋu�ɓ��B����܂łɕK�v�ȃX�s�[�h�A�b�v��

		float m_minSpawnInterval = 0.5f;


		float m_obstacleSpeed = 150.0f;

		ObstaclePool* m_obstaclePool;
		GameMaster* m_gameMaster;

		std::mt19937_64 m_randomEngine;
		std::uniform_int_distribution<int> m_randomGenerator;

		void SpawnObstacle();
	};
}