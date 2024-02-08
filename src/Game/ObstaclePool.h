#pragma once
#include "Framework/GameFramework.h"

namespace FlappyBird
{
	// 障害物の管理クラス
	class ObstaclePool : public Framework::IComponent
	{
	public:
		ObstaclePool(std::shared_ptr<Framework::Object> owner);
		~ObstaclePool();

		void Update(float deltaTime) override;
		void Draw() override;

		// 障害物をプールから取り出す
		std::shared_ptr<Framework::GameObject>& GetObstacle();

	private:
		const size_t POOL_SIZE = 20;
		std::list<std::shared_ptr<Framework::GameObject>> m_obstaclePool;

		void InitObstaclePool();
	};
}