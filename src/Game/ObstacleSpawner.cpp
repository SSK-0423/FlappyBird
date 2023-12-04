#include "pch.h"
#include "ObstacleSpawner.h"
#include "ObstaclePool.h"
#include "Obstacle.h"

using namespace Framework;

namespace FlappyBird
{
	ObstacleSpawner::ObstacleSpawner(Framework::Object* owner) : IComponent(owner)
	{
		m_obstaclePool = m_owner->AddComponent<ObstaclePool>(m_owner);

		// テスト用の障害物を生成
		auto windowSize = Window::GetWindowSize();

		auto& obstacle = m_obstaclePool->GetObstacle();
		obstacle.GetComponent<Transform2D>()->position = { static_cast<float>(windowSize.cx), 500 };
		obstacle.GetComponent<Obstacle>()->SetMoveSpeed(-1.f, 0.f);
	}
	ObstacleSpawner::~ObstacleSpawner()
	{
	}
	void ObstacleSpawner::Update(float deltaTime)
	{
		// ランダムに障害物を生成する
	}
	void ObstacleSpawner::Draw()
	{
	}
}