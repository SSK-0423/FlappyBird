#pragma once
#include "Framework/GameFramework.h"

namespace FlappyBird
{
	// ��Q���̊Ǘ��N���X
	class ObstaclePool : public Framework::IComponent
	{
	public:
		ObstaclePool(std::shared_ptr<Framework::Object> owner);
		~ObstaclePool();

		void Update(float deltaTime) override;
		void Draw() override;

		// ��Q�����v�[��������o��
		std::shared_ptr<Framework::GameObject>& GetObstacle();

	private:
		const size_t POOL_SIZE = 20;
		std::list<std::shared_ptr<Framework::GameObject>> m_obstaclePool;

		void InitObstaclePool();
	};
}