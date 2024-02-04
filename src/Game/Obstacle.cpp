#include "pch.h"
#include "Obstacle.h"

#include "FlappyBird.h"

#include "GameScene.h"
#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace Framework;
using namespace DX12Wrapper;

namespace FlappyBird
{
	// �ÓI�����o�ϐ��̏�����
	float Obstacle::m_judgeLineX = 0.0f;
	float Obstacle::m_currentPlayTime = 0.0f;

	Obstacle::Obstacle(Framework::Object* owner)
		: IComponent(owner)
	{
		m_owner->SetName("Obstacle");
		m_owner->SetTag("Obstacle");

		auto viewportSize = Dx12GraphicsEngine::GetViewport();

		m_transform = m_owner->GetComponent<Transform2D>();
		m_transform->position = { viewportSize.Width / 2.f, viewportSize.Height / 2.f };
		m_transform->scale = { 100.f, viewportSize.Height / 1.f };

		RectCollider* collider = m_owner->AddComponent<RectCollider>(m_owner);
		collider->SetRectSize(m_transform->scale.x, m_transform->scale.y);

		Rigidbody2D* rigidbody = m_owner->AddComponent<Rigidbody2D>(m_owner);
		rigidbody->useGravity = false;

		Sprite* sprite = new Sprite(L"res/texture/dokan_long.png");
		SpriteRenderer* spriteRenderer = m_owner->AddComponent<SpriteRenderer>(m_owner);
		spriteRenderer->SetSprite(sprite);
		spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GAMEOBJECT);
		spriteRenderer->SetLayer(static_cast<UINT>(SPRITE_LAYER::OBSTACLE));

		m_timing = -1.0f;
	}
	Obstacle::~Obstacle()
	{
	}
	void Obstacle::Update(float deltaTime)
	{
		//Transform2D* transform = m_owner->GetComponent<Transform2D>();
		//transform->position.x += m_moveSpeedX * deltaTime;
		//transform->position.y += m_moveSpeedY * deltaTime;

		//// �J�����͈̔͊O�ɏo�����A�N�e�B�u�ɂ���
		//// 1. �J�����̃����_�����O�͈͓��ɓ����Ă��邩�ǂ����𔻒肷��

		//if (transform->position.x < -transform->scale.x)
		//{
		//	// ����炷
		//	m_owner->SetActive(false);
		//}

		if (m_timing < 0.0f)
		{
			return;
		}

		// �m�[�c�̃^�C�~���O�Ɣ��胉�C���̃^�C�~���O�̍����v�Z
		float diff = m_timing - m_currentPlayTime;

		auto viewportSize = Dx12GraphicsEngine::GetViewport();

		// ��ʂ̉E�[���画�胉�C���܂ł̋���
		float distanceX = viewportSize.Width - m_judgeLineX;

		float x = (diff / 1000.f) * distanceX;

		m_transform->position.x = m_judgeLineX + x / 2.f;
	}
	void Obstacle::Draw()
	{
	}
	void Obstacle::SetMoveSpeed(float moveSpeedX, float moveSpeedY)
	{
		m_moveSpeedX = moveSpeedX;
		m_moveSpeedY = moveSpeedY;
	}
	void Obstacle::SetTiming(float timing)
	{
		m_timing = timing;
	}
	void Obstacle::SetCurrentPlayTime(float currentPlayTime)
	{
		m_currentPlayTime = currentPlayTime;
	}
	void Obstacle::SetJudgeLineX(float judgeLineX)
	{
		m_judgeLineX = judgeLineX;
	}
}
