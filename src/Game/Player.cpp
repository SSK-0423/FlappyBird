#include "pch.h"
#include "Player.h"

#include "Framework/GameFramework.h"

#include "GameScene.h"

using namespace Framework;

namespace FlappyBird
{
	Player::Player(Framework::Object* owner)
		: Framework::IComponent(owner), m_jumpVelocity(-5.f) // 左上原点なのでマイナス
	{
		m_owner->SetName("Player");
		m_owner->SetTag("Player");

		// スプライト追加
		Sprite* sprite = new Sprite(L"res/player_jump.png");
		m_owner->AddComponent<SpriteRenderer>(m_owner);
		m_owner->GetComponent<SpriteRenderer>()->SetSprite(sprite);
		m_owner->GetComponent<SpriteRenderer>()->SetDrawMode(SPRITE_DRAW_MODE::GAMEOBJECT);
		m_owner->GetComponent<SpriteRenderer>()->SetLayer(static_cast<UINT>(GAME_SCENE_LAYER::GAMEOBJECT));

		// プレイヤーの位置を設定
		auto windowSize = Window::GetWindowSize();
		Transform2D* transform = m_owner->GetComponent<Transform2D>();
		transform->position = { 200.f, windowSize.cy / 2.f };
		transform->scale = { 75.f, 75.f };

		// コライダー追加
		RectCollider* collider = m_owner->AddComponent<RectCollider>(m_owner);
		collider->SetRectSize(transform->scale.x, transform->scale.y);
		collider->SetOnCollisionCallBack(std::bind(&Player::OnCollision, this, std::placeholders::_1));

		// リジッドボディ追加
		Rigidbody2D* rigidbody = m_owner->AddComponent<Rigidbody2D>(m_owner);
		rigidbody->gravityScale = 1.5f;

		// 効果音追加
		SoundClip* sound = m_owner->AddComponent<SoundClip>(m_owner);
		sound->LoadWavSound(L"res/sound/se_jump3.wav");
	}
	Player::~Player()
	{
	}
	void Player::Update(float deltaTime)
	{
		// プレイヤーの移動制限
		auto windowSize = Window::GetWindowSize();
		Transform2D* transform = m_owner->GetComponent<Transform2D>();

		// 上端制限
		if (transform->position.y < 0.f)
		{
			transform->position.y = 0.f;
			m_owner->GetComponent<Rigidbody2D>()->velocity = { 0.f, 0.f };
		}
		// 下端制限
		else if (transform->position.y > windowSize.cy)
		{
			transform->position.y = windowSize.cy;
			m_owner->GetComponent<Rigidbody2D>()->velocity = { 0.f, 0.f };
		}

		// マウス左クリックでジャンプ
		if (InputSystem::GetMouseButtonDown(MOUSECODE::LEFT))
		{
			m_owner->GetComponent<Rigidbody2D>()->velocity = { 0.f, 0.f };
			m_owner->GetComponent<Rigidbody2D>()->AddForce({ 0.f, m_jumpVelocity }, FORCE_MODE::VELOCITY);
			m_owner->GetComponent<SoundClip>()->Play();
		}
	}
	void Player::Draw()
	{
	}
	void Player::OnCollision(Framework::Collider* other)
	{
		// 障害物に当たったらゲームオーバー
		if (other->GetOwner()->GetTag() == "Obstacle")
		{
			Utility::DebugLog("Game Over\n");
		}
	}
}