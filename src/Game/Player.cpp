#include "pch.h"
#include "Player.h"

#include "Framework/GameFramework.h"

#include "GameScene.h"

using namespace Framework;

namespace FlappyBird
{
	Player::Player(Framework::Object* owner)
		: Framework::IComponent(owner), m_jumpVelocity(-5.f)
	{
		// スプライト追加
		Sprite* sprite = new Sprite(L"res/tile_64.png");
		m_owner->AddComponent<SpriteRenderer>(m_owner);
		m_owner->GetComponent<SpriteRenderer>()->SetSprite(sprite);
		m_owner->GetComponent<SpriteRenderer>()->SetDrawMode(SPRITE_DRAW_MODE::GAMEOBJECT);
		m_owner->GetComponent<SpriteRenderer>()->SetLayer(static_cast<UINT>(GAME_SCENE_LAYER::GAMEOBJECT));

		// コライダー追加
		RectCollider* collider = m_owner->AddComponent<RectCollider>(m_owner);
		collider->SetRectSize(100.f, 100.f);

		// リジッドボディ追加
		Rigidbody2D* rigidbody = m_owner->AddComponent<Rigidbody2D>(m_owner);
		rigidbody->gravityScale = 1.5f;

		// プレイヤーの位置を設定
		auto windowSize = Window::GetWindowSize();
		m_owner->GetComponent<Transform2D>()->position = { 200.f, windowSize.cy / 2.f };
	}
	Player::~Player()
	{
	}
	void Player::Update(float deltaTime)
	{
		// マウス左クリックでジャンプ
		if (InputSystem::GetMouseButtonDown(MOUSECODE::LEFT))
		{
			// 左上原点なので、下方向に力を加える
			m_owner->GetComponent<Rigidbody2D>()->velocity = { 0.f, 0.f };
			m_owner->GetComponent<Rigidbody2D>()->AddForce({ 0.f, m_jumpVelocity }, FORCE_MODE::VELOCITY);
		}
	}
	void Player::Draw()
	{
	}
	void Player::OnCollision(Framework::Collider* other)
	{
	}
}