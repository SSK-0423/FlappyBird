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

		// 現在時刻(ms)をシード値として乱数生成器を初期化
		auto now = std::chrono::system_clock::now();
		auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
		m_randomEngine = std::mt19937_64(nowMs);
		m_randomGenerator = std::uniform_int_distribution<unsigned int>(SPACE, Window::GetWindowSize().cy - SPACE);
	}
	ObstacleSpawner::~ObstacleSpawner()
	{
	}
	void ObstacleSpawner::Update(float deltaTime)
	{
		m_spawnTimer += deltaTime;

		if (m_spawnTimer >= m_spawnInverval)
		{
			SpawnObstacle();
		}
	}
	void ObstacleSpawner::Draw()
	{
	}
	void ObstacleSpawner::SpawnObstacle()
	{
		// スポーンタイマーをリセット
		m_spawnTimer = 0.0f;

		// 土管の隙間のY座標をランダムに決める
		unsigned int randomY = m_randomGenerator(m_randomEngine);

		auto windowSize = Window::GetWindowSize();

		// 土管を設置
		auto& overObstacle = m_obstaclePool->GetObstacle();
		Transform2D* overObstacleTransform = overObstacle.GetComponent<Transform2D>();
		overObstacleTransform->position = {
			static_cast<float>(windowSize.cx),
			randomY - overObstacleTransform->scale.y / 2.f - SPACE / 2.f };
		overObstacleTransform->angle = 180.f;

		auto& underObstacle = m_obstaclePool->GetObstacle();
		Transform2D* underObstacleTransform = underObstacle.GetComponent<Transform2D>();
		underObstacleTransform->position = {
			static_cast<float>(windowSize.cx), 
			randomY + overObstacleTransform->scale.y / 2.f + SPACE / 2.f };

		// 土管の移動速度を設定
		overObstacle.GetComponent<Obstacle>()->SetMoveSpeed(-1.f, 0.f);
		underObstacle.GetComponent<Obstacle>()->SetMoveSpeed(-1.f, 0.f);
	}
}