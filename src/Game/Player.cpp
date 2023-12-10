#include "pch.h"
#include "Player.h"

#include "Framework/GameFramework.h"

#include "GameScene.h"
#include "GameMaster.h"

using namespace Framework;

namespace FlappyBird
{
	Player::Player(Framework::Object* owner)
		: Framework::IComponent(owner), m_jumpVelocity(-5.f), // 左上原点なのでマイナス
		m_elapsedTime(0.f), m_gameReadyAnimationInterval(0.675f), m_isAlive(true)
	{
		m_gameMaster = GameObjectManager::FindObject("GameMaster")->GetComponent<GameMaster>();

		m_jumpSprite = std::make_shared<Sprite>(L"res/texture/player_jump.png");
		m_fallSprite = std::make_shared<Sprite>(L"res/texture/player_fall.png");

		m_owner->SetName("Player");
		m_owner->SetTag("Player");

		// スプライト追加
		SpriteRenderer* spriteRenderer = m_owner->AddComponent<SpriteRenderer>(m_owner);
		spriteRenderer->SetSprite(m_fallSprite);
		spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GAMEOBJECT);
		spriteRenderer->SetLayer(static_cast<UINT>(GAME_SCENE_LAYER::GAMEOBJECT));

		// プレイヤーの位置を設定
		auto windowSize = Window::GetWindowSize();
		Transform2D* transform = m_owner->GetComponent<Transform2D>();
		transform->position = { 200.f, 50.f };
		transform->scale = { 75.f, 75.f };

		// コライダー追加
		RectCollider* collider = m_owner->AddComponent<RectCollider>(m_owner);
		collider->SetRectSize(transform->scale.x * 0.7f, transform->scale.y * 0.7f);
		collider->SetOnCollisionCallBack(std::bind(&Player::OnCollision, this, std::placeholders::_1));

		// リジッドボディ追加
		Rigidbody2D* rigidbody = m_owner->AddComponent<Rigidbody2D>(m_owner);
		rigidbody->gravityScale = 1.5f;

		// 効果音追加
		SoundClip* sound = m_owner->AddComponent<SoundClip>(m_owner);
		sound->LoadWavSound(L"res/sound/se_jump3.wav");
	}
	void Player::Update(float deltaTime)
	{
		// 生存中のみ更新
		switch (m_gameMaster->GetGameState())
		{
		case GAME_STATE::READY:
			GameReadyAnimation(deltaTime);
			break;
		case GAME_STATE::PLAYING:
			Move(deltaTime);
			ChangeSprite();
			break;
		case GAME_STATE::GAMEOVER:
			GameOverAnimation(deltaTime);
			break;
		default:
			break;
		}
	}
	void Player::Draw()
	{
	}
	void Player::OnCollision(Framework::Collider* other)
	{
		// 生存中かつ障害物に当たったらゲームオーバー
		if (m_isAlive && other->GetOwner()->GetTag() == "Obstacle")
		{
			Utility::DebugLog("Game Over\n");
			OnDead();
		}
	}
	void Player::ChangeSprite()
	{
		// 落下中は落下スプライトを表示
		if (m_owner->GetComponent<Rigidbody2D>()->velocity.y >= 0.f)
		{
			m_owner->GetComponent<SpriteRenderer>()->SetSprite(m_fallSprite);
		}
		// 上昇中は上昇スプライトを表示
		else
		{
			m_owner->GetComponent<SpriteRenderer>()->SetSprite(m_jumpSprite);
		}
	}
	void Player::Move(float deltaTime)
	{
		LimitPosition();

		// マウス左クリックでジャンプ
		if (InputSystem::GetMouseButtonDown(MOUSECODE::LEFT))
		{
			Jump();
		}
	}

	void Player::Jump()
	{
		m_owner->GetComponent<Rigidbody2D>()->velocity = { 0.f, 0.f };
		m_owner->GetComponent<Rigidbody2D>()->AddForce({ 0.f, m_jumpVelocity }, FORCE_MODE::VELOCITY);
		m_owner->GetComponent<SoundClip>()->Play();
	}

	void Player::LimitPosition()
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
	}

	void Player::GameReadyAnimation(float deltaTime)
	{
		// ゲーム開始演出
		// 一定間隔でジャンプする
		m_elapsedTime += deltaTime;
		if (m_elapsedTime >= m_gameReadyAnimationInterval)
		{
			Jump();
			m_elapsedTime = 0.f;
		}
		ChangeSprite();
	}

	void Player::GameOverAnimation(float deltaTime)
	{
		// ゲームオーバー演出
		Transform2D* transform = m_owner->GetComponent<Transform2D>();
		transform->position.y += 2.5f;
		transform->angle += 2.5f;
	}

	bool Player::IsDead()
	{
		return !m_isAlive;
	}

	void Player::OnDead()
	{
		m_isAlive = false;

		m_gameMaster->ChangeState(GAME_STATE::GAMEOVER);

		// Rigidbodyの影響を無効化
		m_owner->GetComponent<Rigidbody2D>()->SetActive(false);

		// ダメージ音声再生
		std::unique_ptr<SoundClip> damageSound = std::make_unique<SoundClip>(nullptr);
		damageSound->LoadWavSound(L"res/sound/se_damage5.wav");
		damageSound->Play(true);

		// 落下音声再生
		std::unique_ptr<SoundClip> sound = std::make_unique<SoundClip>(nullptr);
		sound->LoadWavSound(L"res/sound/水に落下（ヒューポチャン）.wav");
		sound->Play();
	}
}