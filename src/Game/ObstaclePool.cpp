#include "pch.h"
#include "ObstaclePool.h"
#include "Obstacle.h"

using namespace Framework;

namespace FlappyBird
{
	ObstaclePool::ObstaclePool(std::shared_ptr<Object> owner) : IComponent(owner)
	{
		InitObstaclePool();
	}
	ObstaclePool::~ObstaclePool()
	{
		m_obstaclePool.clear();
	}
	void ObstaclePool::Update(float deltaTime)
	{
	}
	void ObstaclePool::Draw()
	{
	}
	std::shared_ptr<GameObject>& ObstaclePool::GetObstacle()
	{
		// 使用可能な障害物を探して参照を返す
		for (auto& obstacle : m_obstaclePool)
		{
			if (!obstacle->GetActive())
			{
				obstacle->SetActive(true);
				return obstacle;
			}
		}

		// 使用可能な障害物がない場合は新たに生成する
		m_obstaclePool.push_back(std::shared_ptr<GameObject>(new GameObject()));
		m_obstaclePool.back()->AddComponent<Obstacle>(m_obstaclePool.back());
		m_owner.lock()->AddChild(m_obstaclePool.back());

		return m_obstaclePool.back();
	}
	void ObstaclePool::InitObstaclePool()
	{
		// 障害物を生成してプールに追加する
		m_obstaclePool.resize(POOL_SIZE);
		for (auto& obstacle : m_obstaclePool)
		{
			obstacle = std::shared_ptr<GameObject>(new GameObject());
			obstacle->AddComponent<Obstacle>(obstacle);
			obstacle->SetActive(false);
			m_owner.lock()->AddChild(obstacle);
		}
	}
}

