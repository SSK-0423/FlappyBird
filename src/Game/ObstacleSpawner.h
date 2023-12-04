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

		ObstaclePool* m_obstaclePool;
	};
}