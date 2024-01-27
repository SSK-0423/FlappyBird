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

		// ���ݎ���(ms)���V�[�h�l�Ƃ��ė����������������
		auto now = std::chrono::system_clock::now();
		auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
		m_randomEngine = std::mt19937_64(nowMs);
		m_randomGenerator = std::uniform_int_distribution<int>(SPACE, static_cast<int>(Dx12GraphicsEngine::GetViewport().Height - SPACE));

		// �y�ǂ̏����ʒu��ݒ�
		m_lastSpawnY = m_randomGenerator(m_randomEngine);

		// �y�ǂ̕��擾
		float obstacleScaleX = 100.0f;

		// �ŏ��y�ǐ����Ԋu = �y�ǂ̉��� / �y�ǂ̈ړ��X�s�[�h
		// �y�ǂ̐����ʒu�͉�ʂ̉E�[+�y�ǂ̉����Ȃ̂ŁA�������ړ��X�s�[�h�Ŋ����
		// �������ꂽ�y�ǂ���ʓ��Ɏ��܂�܂ł̎��Ԃ��킩��
		m_minSpawnInterval = obstacleScaleX / m_obstacleSpeed;
		// 1��̃X�s�[�h�A�b�v�̕ω���
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
		// ��莞�Ԍo�߂���Ɠy�ǂ̐����Ԋu��Z������
		// �������A�ŏ������Ԋu�������Ȃ��悤���āA�y�ǂ��d�Ȃ�Ȃ��悤�ɂ���
		if (m_spawnInverval > m_minSpawnInterval &&
			m_spawnSpeedUpTimer >= m_spawnSpeedUpInterval)
		{
			// �덷���l�����čŏ������Ԋu�������Ȃ��悤�ɂ���
			m_spawnInverval = std::max(m_spawnInverval - m_spawnSpeedUpDiff, m_minSpawnInterval);
			m_spawnSpeedUpTimer = 0.f;
		}
	}
	void ObstacleSpawner::Draw()
	{
	}
	void ObstacleSpawner::SpawnObstacle()
	{
		// �X�|�[���^�C�}�[�����Z�b�g
		m_spawnTimer = 0.0f;

		auto viewportSize = Dx12GraphicsEngine::GetViewport();

		// ��΂ɔ������Ȃ��z�u�ɂȂ�Ȃ��悤�ɂ���
		// �y�ǂ̐����Ԋu���ŏ������Ԋu�ɋ߂��قǁA�y�ǐ������̗����������߂�
		// �ŏ������Ԋu�̎��ɓ��������ɐ�����������̂�h�����߂ɁA�Ō�ɏ������l�𑫂�
		int diff = SPACE * std::abs(m_spawnInverval - m_minSpawnInterval) + SPACE / 5.f;
		int minY = std::max(SPACE, m_lastSpawnY - diff);
		int maxY = std::min(m_lastSpawnY + diff, static_cast<int>(viewportSize.Height - SPACE));

		m_randomGenerator.param(std::uniform_int_distribution<int>::param_type(minY, maxY));
		// �y�ǂ̌��Ԃ�Y���W�������_���Ɍ��߂�
		int randomY = m_randomGenerator(m_randomEngine);

		// �y�ǂ�ݒu
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

		// �y�ǂ̈ړ����x��ݒ�
		overObstacle.GetComponent<Obstacle>()->SetMoveSpeed(-m_obstacleSpeed, 0.f);
		underObstacle.GetComponent<Obstacle>()->SetMoveSpeed(-m_obstacleSpeed, 0.f);

		m_lastSpawnY = randomY;

		Utility::DebugLog("minY: %d, maxY: %d, randomY: %d\n", minY, maxY, randomY);
	}
}