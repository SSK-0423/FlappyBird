#include "pch.h"
#include "Player.h"

#include "GameScene.h"
#include "GameMaster.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace Framework;
using namespace DX12Wrapper;

namespace FlappyBird
{
	Player::Player(std::shared_ptr<Framework::Object> owner)
		: Framework::IComponent(owner), m_jumpVelocity(-750.f), // ���㌴�_�Ȃ̂Ń}�C�i�X
		m_elapsedTime(0.f), m_gameReadyAnimationInterval(0.65f), m_isAlive(true)
	{
		m_gameMaster = GameObjectManager::FindObject("GameMaster")->GetComponent<GameMaster>();

		m_stateSprites[STATE::JUMP] = std::make_shared<Sprite>(L"res/texture/player_jump.png");
		m_stateSprites[STATE::FALL] = std::make_shared<Sprite>(L"res/texture/player_fall.png");
		m_stateSprites[STATE::DEAD] = std::make_shared<Sprite>(L"res/texture/player_dead.png");

		m_owner.lock()->SetName("Player");
		m_owner.lock()->SetTag("Player");

		// �X�v���C�g�ǉ�
		SpriteRenderer* spriteRenderer = m_owner.lock()->AddComponent<SpriteRenderer>(m_owner.lock());
		spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GAMEOBJECT);
		spriteRenderer->SetLayer(static_cast<UINT>(GAME_SCENE_LAYER::GAMEOBJECT));
		spriteRenderer->AddSprite(m_stateSprites[STATE::JUMP]);
		spriteRenderer->AddSprite(m_stateSprites[STATE::FALL]);
		spriteRenderer->AddSprite(m_stateSprites[STATE::DEAD]);

		// �v���C���[�̈ʒu��ݒ�
		auto& viewportSize = Dx12GraphicsEngine::GetViewport();
		Transform2D* transform = m_owner.lock()->GetComponent<Transform2D>();
		transform->position = { 200.f, viewportSize.Height / 2.f };
		transform->scale = { 75.f, 75.f };

		// �R���C�_�[�ǉ�
		RectCollider* collider = m_owner.lock()->AddComponent<RectCollider>(m_owner.lock());
		collider->SetRectSize(transform->scale.x * 0.2f, transform->scale.y * 0.6f);
		collider->SetOnCollisionCallBack(std::bind(&Player::OnCollision, this, std::placeholders::_1));

		// ���W�b�h�{�f�B�ǉ�
		Rigidbody2D* rigidbody = m_owner.lock()->AddComponent<Rigidbody2D>(m_owner.lock());
		rigidbody->gravityScale = 5.0f;

		// ���ʉ��ǉ�
		SoundClip* sound = m_owner.lock()->AddComponent<SoundClip>(m_owner.lock());
		sound->LoadWavSound(L"res/sound/jump.wav");
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
			Editor::DebugLog("Game Over");
			//OnDead();
		}
	}
	void Player::ChangeSprite()
	{
		// �������͗����X�v���C�g��\��
		if (m_owner.lock()->GetComponent<Rigidbody2D>()->velocity.y >= 0.f)
		{
			m_owner.lock()->GetComponent<SpriteRenderer>()->ChangeRenderSprite(static_cast<size_t>(STATE::FALL));
		}
		// �㏸���͏㏸�X�v���C�g��\��
		else
		{
			m_owner.lock()->GetComponent<SpriteRenderer>()->ChangeRenderSprite(static_cast<size_t>(STATE::JUMP));
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
		m_owner.lock()->GetComponent<Rigidbody2D>()->velocity = { 0.f, 0.f };
		m_owner.lock()->GetComponent<Rigidbody2D>()->AddForce({ 0.f, m_jumpVelocity }, FORCE_MODE::VELOCITY);
		m_owner.lock()->GetComponent<SoundClip>()->Play();
	}

	void Player::LimitPosition()
	{
		// �v���C���[�̈ړ�����
		auto& viewport = Dx12GraphicsEngine::GetViewport();
		Transform2D* transform = m_owner.lock()->GetComponent<Transform2D>();

		// ��[����
		if (transform->position.y < 0.f)
		{
			transform->position.y = 0.f;
			m_owner.lock()->GetComponent<Rigidbody2D>()->velocity = { 0.f, 0.f };
		}
		// ���[����
		else if (transform->position.y > viewport.Height)
		{
			transform->position.y = viewport.Height;
			m_owner.lock()->GetComponent<Rigidbody2D>()->velocity = { 0.f, 0.f };
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
		m_owner.lock()->GetComponent<SpriteRenderer>()->ChangeRenderSprite(static_cast<size_t>(STATE::DEAD));

		Transform2D* transform = m_owner.lock()->GetComponent<Transform2D>();
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
		m_owner.lock()->GetComponent<Rigidbody2D>()->SetActive(false);

		// �_���[�W�����Đ�
		std::unique_ptr<SoundClip> damageSound = std::make_unique<SoundClip>(nullptr);
		damageSound->LoadWavSound(L"res/sound/damage.wav");
		damageSound->Play(0.1f, 0.0f, true);

		// ���������Đ�
		std::unique_ptr<SoundClip> sound = std::make_unique<SoundClip>(nullptr);
		sound->LoadWavSound(L"res/sound/fall.wav");
		sound->Play(0.1f, 0.0f);
	}
}