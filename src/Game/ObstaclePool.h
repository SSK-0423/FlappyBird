#pragma once
#include "Framework/GameFramework.h"

namespace FlappyBird
{
	// ��Q���̊Ǘ��N���X
	class ObstaclePool : public Framework::IComponent
	{
	public:
		ObstaclePool(Framework::Object* owner);
		~ObstaclePool();

		void Update(float deltaTime) override;
		void Draw() override;

		// ��Q�����v�[��������o��
		Framework::GameObject& GetObstacle();

	private:
		const size_t POOL_SIZE = 50;
		std::list<Framework::GameObject> m_obstaclePool;

		void InitObstaclePool();
	};
}