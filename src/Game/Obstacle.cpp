#include "pch.h"
#include "Obstacle.h"

#include "GameScene.h"
#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace Framework;
using namespace DX12Wrapper;

namespace FlappyBird
{
	Obstacle::Obstacle(Framework::Object* owner)
		: IComponent(owner)
	{
		m_owner->SetName("Obstacle");
		m_owner->SetTag("Obstacle");

		auto viewportSize = Dx12GraphicsEngine::GetViewport();

		Transform2D* transform = m_owner->GetComponent<Transform2D>();
		transform->position = { 500.f, 300.f };
		transform->scale = { 100.f, viewportSize.Height / 1.f };

		RectCollider* collider = m_owner->AddComponent<RectCollider>(m_owner);
		collider->SetRectSize(transform->scale.x, transform->scale.y);

		Rigidbody2D* rigidbody = m_owner->AddComponent<Rigidbody2D>(m_owner);
		rigidbody->useGravity = false;

		Sprite* sprite = new Sprite(L"res/texture/dokan_long.png");
		SpriteRenderer* spriteRenderer = m_owner->AddComponent<SpriteRenderer>(m_owner);
		spriteRenderer->SetSprite(sprite);
		spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GAMEOBJECT);
		spriteRenderer->SetLayer(static_cast<UINT>(GAME_SCENE_LAYER::GAMEOBJECT));
	}
	Obstacle::~Obstacle()
	{
	}
	void Obstacle::Update(float deltaTime)
	{
		Transform2D* transform = m_owner->GetComponent<Transform2D>();
		transform->position.x += m_moveSpeedX * deltaTime;
		transform->position.y += m_moveSpeedY * deltaTime;

		// カメラの範囲外に出たら非アクティブにする
		// 1. カメラのレンダリング範囲内に入っているかどうかを判定する

		if (transform->position.x < -transform->scale.x)
		{
			// 音を鳴らす
			m_owner->SetActive(false);
		}

	}
	void Obstacle::Draw()
	{
	}
	void Obstacle::SetMoveSpeed(float moveSpeedX, float moveSpeedY)
	{
		m_moveSpeedX = moveSpeedX;
		m_moveSpeedY = moveSpeedY;
	}
}
