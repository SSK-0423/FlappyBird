#include "pch.h"
#include "Player.h"

#include "Framework/GameFramework.h"

#include "GameScene.h"
#include "GameMaster.h"

using namespace Framework;

namespace FlappyBird
{
	Player::Player(Framework::Object* owner)
		: Framework::IComponent(owner), m_jumpVelocity(-5.f), // ���㌴�_�Ȃ̂Ń}�C�i�X
		m_elapsedTime(0.f), m_gameReadyAnimationInterval(0.675f), m_isAlive(true)
	{
		m_gameMaster = GameObjectManager::FindObject("GameMaster")->GetComponent<GameMaster>();

		m_jumpSprite = std::make_shared<Sprite>(L"res/texture/player_jump.png");
		m_fallSprite = std::make_shared<Sprite>(L"res/texture/player_fall.png");

		m_owner->SetName("Player");
		m_owner->SetTag("Player");

		// �X�v���C�g�ǉ�
		SpriteRenderer* spriteRenderer = m_owner->AddComponent<SpriteRenderer>(m_owner);
		spriteRenderer->SetSprite(m_fallSprite);
		spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GAMEOBJECT);
		spriteRenderer->SetLayer(static_cast<UINT>(GAME_SCENE_LAYER::GAMEOBJECT));

		// �v���C���[�̈ʒu��ݒ�
		auto windowSize = Window::GetWindowSize();
		Transform2D* transform = m_owner->GetComponent<Transform2D>();
		transform->position = { 200.f, 50.f };
		transform->scale = { 75.f, 75.f };

		// �R���C�_�[�ǉ�
		RectCollider* collider = m_owner->AddComponent<RectCollider>(m_owner);
		collider->SetRectSize(transform->scale.x * 0.7f, transform->scale.y * 0.7f);
		collider->SetOnCollisionCallBack(std::bind(&Player::OnCollision, this, std::placeholders::_1));

		// ���W�b�h�{�f�B�ǉ�
		Rigidbody2D* rigidbody = m_owner->AddComponent<Rigidbody2D>(m_owner);
		rigidbody->gravityScale = 1.5f;

		// ���ʉ��ǉ�
		SoundClip* sound = m_owner->AddComponent<SoundClip>(m_owner);
		sound->LoadWavSound(L"res/sound/se_jump3.wav");
	}
	void Player::Update(float deltaTime)
	{
		// �������̂ݍX�V
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
		// ����������Q���ɓ���������Q�[���I�[�o�[
		if (m_isAlive && other->GetOwner()->GetTag() == "Obstacle")
		{
			Utility::DebugLog("Game Over\n");
			OnDead();
		}
	}
	void Player::ChangeSprite()
	{
		// �������͗����X�v���C�g��\��
		if (m_owner->GetComponent<Rigidbody2D>()->velocity.y >= 0.f)
		{
			m_owner->GetComponent<SpriteRenderer>()->SetSprite(m_fallSprite);
		}
		// �㏸���͏㏸�X�v���C�g��\��
		else
		{
			m_owner->GetComponent<SpriteRenderer>()->SetSprite(m_jumpSprite);
		}
	}
	void Player::Move(float deltaTime)
	{
		LimitPosition();

		// �}�E�X���N���b�N�ŃW�����v
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
		// �v���C���[�̈ړ�����
		auto windowSize = Window::GetWindowSize();
		Transform2D* transform = m_owner->GetComponent<Transform2D>();

		// ��[����
		if (transform->position.y < 0.f)
		{
			transform->position.y = 0.f;
			m_owner->GetComponent<Rigidbody2D>()->velocity = { 0.f, 0.f };
		}
		// ���[����
		else if (transform->position.y > windowSize.cy)
		{
			transform->position.y = windowSize.cy;
			m_owner->GetComponent<Rigidbody2D>()->velocity = { 0.f, 0.f };
		}
	}

	void Player::GameReadyAnimation(float deltaTime)
	{
		// �Q�[���J�n���o
		// ���Ԋu�ŃW�����v����
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
		// �Q�[���I�[�o�[���o
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

		// Rigidbody�̉e���𖳌���
		m_owner->GetComponent<Rigidbody2D>()->SetActive(false);

		// �_���[�W�����Đ�
		std::unique_ptr<SoundClip> damageSound = std::make_unique<SoundClip>(nullptr);
		damageSound->LoadWavSound(L"res/sound/se_damage5.wav");
		damageSound->Play(true);

		// ���������Đ�
		std::unique_ptr<SoundClip> sound = std::make_unique<SoundClip>(nullptr);
		sound->LoadWavSound(L"res/sound/���ɗ����i�q���[�|�`�����j.wav");
		sound->Play();
	}
}