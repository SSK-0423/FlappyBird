#include "pch.h"
#include "ObstacleSpawner.h"
#include "ObstaclePool.h"
#include "Obstacle.h"
#include "GameMaster.h"

using namespace Framework;

namespace FlappyBird
{
	ObstacleSpawner::ObstacleSpawner(Framework::Object* owner) : IComponent(owner)
	{
		m_obstaclePool = m_owner->AddComponent<ObstaclePool>(m_owner);
		m_gameMaster = GameObjectManager::FindObject("GameMaster")->GetComponent<GameMaster>();

		// ���ݎ���(ms)���V�[�h�l�Ƃ��ė����������������
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
		m_speedUpTimer += deltaTime;

		if (m_spawnTimer >= m_spawnInverval)
		{
			SpawnObstacle();
		}
		if (m_obstacleSpeed < m_maxObstacleSpeed && 
			m_speedUpTimer >= m_speedUpInterval)
		{
			m_obstacleSpeed += 25.0f;
			m_speedUpTimer = 0.0f;
		}
	}
	void ObstacleSpawner::Draw()
	{
	}
	void ObstacleSpawner::SpawnObstacle()
	{
		// �X�|�[���^�C�}�[�����Z�b�g
		m_spawnTimer = 0.0f;

		// �y�ǂ̌��Ԃ�Y���W�������_���Ɍ��߂�
		unsigned int randomY = m_randomGenerator(m_randomEngine);

		auto windowSize = Window::GetWindowSize();

		// �y�ǂ�ݒu
		auto& overObstacle = m_obstaclePool->GetObstacle();
		Transform2D* overObstacleTransform = overObstacle.GetComponent<Transform2D>();
		overObstacleTransform->position = {
			static_cast<float>(windowSize.cx) + overObstacleTransform->scale.x / 2.f,
			randomY - overObstacleTransform->scale.y / 2.f - SPACE / 2.f };
		overObstacleTransform->angle = 180.f;

		auto& underObstacle = m_obstaclePool->GetObstacle();
		Transform2D* underObstacleTransform = underObstacle.GetComponent<Transform2D>();
		underObstacleTransform->position = {
			static_cast<float>(windowSize.cx) + underObstacleTransform->scale.x / 2.f,
			randomY + overObstacleTransform->scale.y / 2.f + SPACE / 2.f };

		// �y�ǂ̈ړ����x��ݒ�
		overObstacle.GetComponent<Obstacle>()->SetMoveSpeed(-m_obstacleSpeed, 0.f);
		underObstacle.GetComponent<Obstacle>()->SetMoveSpeed(-m_obstacleSpeed, 0.f);
	}
}