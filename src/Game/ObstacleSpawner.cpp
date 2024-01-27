#include "pch.h"
#include "ObstacleSpawner.h"
#include "ObstaclePool.h"
#include "Obstacle.h"
#include "GameMaster.h"
#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace Framework;
using namespace DX12Wrapper;

namespace FlappyBird
{
	ObstacleSpawner::ObstacleSpawner(Framework::Object* owner) : IComponent(owner)
	{
		m_owner->SetName("ObstacleSpawner");

		m_obstaclePool = m_owner->AddComponent<ObstaclePool>(m_owner);
		m_gameMaster = GameObjectManager::FindObject("GameMaster")->GetComponent<GameMaster>();

		// 現在時刻(ms)をシード値として乱数生成器を初期化
		auto now = std::chrono::system_clock::now();
		auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
		m_randomEngine = std::mt19937_64(nowMs);
		m_randomGenerator = std::uniform_int_distribution<int>(SPACE, static_cast<int>(Dx12GraphicsEngine::GetViewport().Height - SPACE));

		// 土管の初期位置を設定
		m_lastSpawnY = m_randomGenerator(m_randomEngine);

		// 土管の幅取得
		float obstacleScaleX = 100.0f;

		// 最小土管生成間隔 = 土管の横幅 / 土管の移動スピード
		// 土管の生成位置は画面の右端+土管の横幅なので、横幅を移動スピードで割ると
		// 生成された土管が画面内に収まるまでの時間がわかる
		m_minSpawnInterval = obstacleScaleX / m_obstacleSpeed;
		// 1回のスピードアップの変化量
		m_spawnSpeedUpDiff = std::abs(m_spawnInverval - m_minSpawnInterval) / m_spawnSpeedUpStep;
	}
	ObstacleSpawner::~ObstacleSpawner()
	{
	}
	void ObstacleSpawner::Update(float deltaTime)
	{
		m_spawnTimer += deltaTime;
		m_spawnSpeedUpTimer += deltaTime;

		if (m_spawnTimer >= m_spawnInverval)
		{
			SpawnObstacle();
		}
		// 一定時間経過すると土管の生成間隔を短くする
		// ただし、最小生成間隔を下回らないようして、土管が重ならないようにする
		if (m_spawnInverval > m_minSpawnInterval &&
			m_spawnSpeedUpTimer >= m_spawnSpeedUpInterval)
		{
			// 誤差を考慮して最小生成間隔を下回らないようにする
			m_spawnInverval = std::max(m_spawnInverval - m_spawnSpeedUpDiff, m_minSpawnInterval);
			m_spawnSpeedUpTimer = 0.f;
		}
	}
	void ObstacleSpawner::Draw()
	{
	}
	void ObstacleSpawner::SpawnObstacle()
	{
		// スポーンタイマーをリセット
		m_spawnTimer = 0.0f;

		auto viewportSize = Dx12GraphicsEngine::GetViewport();

		// 絶対に避けられない配置にならないようにする
		// 土管の生成間隔が最小生成間隔に近いほど、土管生成時の乱数幅を狭める
		// 最小生成間隔の時に同じ高さに生成し続けるのを防ぐために、最後に小さい値を足す
		int diff = SPACE * std::abs(m_spawnInverval - m_minSpawnInterval) + SPACE / 5.f;
		int minY = std::max(SPACE, m_lastSpawnY - diff);
		int maxY = std::min(m_lastSpawnY + diff, static_cast<int>(viewportSize.Height - SPACE));

		m_randomGenerator.param(std::uniform_int_distribution<int>::param_type(minY, maxY));
		// 土管の隙間のY座標をランダムに決める
		int randomY = m_randomGenerator(m_randomEngine);

		// 土管を設置
		auto& overObstacle = m_obstaclePool->GetObstacle();
		Transform2D* overObstacleTransform = overObstacle.GetComponent<Transform2D>();
		overObstacleTransform->position = {
			static_cast<float>(viewportSize.Width) + overObstacleTransform->scale.x / 2.f,
			randomY - overObstacleTransform->scale.y / 2.f - SPACE / 2.f };
		overObstacleTransform->angle = 180.f;

		auto& underObstacle = m_obstaclePool->GetObstacle();
		Transform2D* underObstacleTransform = underObstacle.GetComponent<Transform2D>();
		underObstacleTransform->position = {
			static_cast<float>(viewportSize.Width) + underObstacleTransform->scale.x / 2.f,
			randomY + overObstacleTransform->scale.y / 2.f + SPACE / 2.f };

		// 土管の移動速度を設定
		overObstacle.GetComponent<Obstacle>()->SetMoveSpeed(-m_obstacleSpeed, 0.f);
		underObstacle.GetComponent<Obstacle>()->SetMoveSpeed(-m_obstacleSpeed, 0.f);

		m_lastSpawnY = randomY;

		Utility::DebugLog("minY: %d, maxY: %d, randomY: %d\n", minY, maxY, randomY);
	}
}