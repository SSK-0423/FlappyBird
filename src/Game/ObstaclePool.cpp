#include "pch.h"
#include "ObstaclePool.h"
#include "Obstacle.h"

using namespace Framework;

namespace FlappyBird
{
	ObstaclePool::ObstaclePool(Object* owner) : IComponent(owner)
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
	Framework::GameObject& ObstaclePool::GetObstacle()
	{
		// �g�p�\�ȏ�Q����T���ĎQ�Ƃ�Ԃ�
		for (auto& obstacle : m_obstaclePool)
		{
			if (!obstacle.GetActive())
			{
				obstacle.SetActive(true);
				return obstacle;
			}
		}

		// �g�p�\�ȏ�Q�����Ȃ��ꍇ�͐V���ɐ�������
		m_obstaclePool.push_back(GameObject());
		m_obstaclePool.back().AddComponent<Obstacle>(&m_obstaclePool.back());

		return m_obstaclePool.back();
	}
	void ObstaclePool::InitObstaclePool()
	{
		// ��Q���𐶐����ăv�[���ɒǉ�����
		m_obstaclePool.resize(POOL_SIZE);
		for (auto& obstacle : m_obstaclePool)
		{
			obstacle.AddComponent<Obstacle>(&obstacle);
			obstacle.SetActive(false);
			m_owner->AddChild(&obstacle);
		}
	}
}

