#include "pch.h"
#include "Player.h"

#include "GameScene.h"
#include "GameMaster.h"
#include "MusicPlayer.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace Framework;
using namespace DX12Wrapper;

namespace FlappyBird
{
	Player::Player(std::shared_ptr<Framework::Object> owner)
		: Framework::IComponent(owner), m_jumpVelocity(-550.f), // ���㌴�_�Ȃ̂Ń}�C�i�X
		m_elapsedTime(0.f), m_gameReadyAnimationInterval(0.65f), m_isAlive(true)
	{
		m_gameMaster = GameObjectManager::FindObject("GameMaster")->GetComponent<GameMaster>();

		m_stateSprites[STATE::JUMP] = std::make_shared<Sprite>(L"res/texture/player_jump.png");
		m_stateSprites[STATE::FALL] = std::make_shared<Sprite>(L"res/texture/player_fall.png");
		m_stateSprites[STATE::DEAD] = std::make_shared<Sprite>(L"res/texture/player_dead.png");

		m_owner.lock()->SetName("Player");
		m_owner.lock()->SetTag("Player");

		// �X�v���C�g�ǉ�
		m_spriteRenderer = m_owner.lock()->AddComponent<SpriteRenderer>(m_owner.lock());
		m_spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GAMEOBJECT);
		m_spriteRenderer->SetLayer(static_cast<UINT>(GAME_SCENE_LAYER::GAMEOBJECT));
		m_spriteRenderer->AddSprite(m_stateSprites[STATE::JUMP]);
		m_spriteRenderer->AddSprite(m_stateSprites[STATE::FALL]);
		m_spriteRenderer->AddSprite(m_stateSprites[STATE::DEAD]);

		// �v���C���[�̈ʒu��ݒ�
		auto& viewportSize = Dx12GraphicsEngine::GetViewport();
		m_transform = m_owner.lock()->GetComponent<Transform2D>();
		m_transform->position = { 200.f, viewportSize.Height / 2.f };
		m_transform->scale = { 75.f, 75.f };

		// �R���C�_�[�ǉ�
		RectCollider* collider = m_owner.lock()->AddComponent<RectCollider>(m_owner.lock());
		collider->SetRectSize(m_transform->scale.x * 0.2f, m_transform->scale.y * 0.6f);
		collider->SetOnCollisionCallBack(std::bind(&Player::OnCollision, this, std::placeholders::_1));

		// ���W�b�h�{�f�B�ǉ�
		m_rigidbody = m_owner.lock()->AddComponent<Rigidbody2D>(m_owner.lock());
		m_rigidbody->gravityScale = 5.0f;

		// SE�p�̃I�u�W�F�N�g�ǉ�
		// �W�����vSE
		std::shared_ptr<GameObject> jumpSoundObject = std::shared_ptr<GameObject>(new GameObject());
		jumpSoundObject->SetName("JumpSound");
		m_jumpSound = jumpSoundObject->AddComponent<SoundClip>(jumpSoundObject);
		m_jumpSound->LoadWavSound(L"res/sound/jump.wav");
		// �_���[�WSE
		std::shared_ptr<GameObject> damageSoundObject = std::shared_ptr<GameObject>(new GameObject());
		damageSoundObject->SetName("DamageSound");
		m_damageSound = damageSoundObject->AddComponent<SoundClip>(damageSoundObject);
		m_damageSound->LoadWavSound(L"res/sound/damage.wav");
		// ����SE
		std::shared_ptr<GameObject> fallSoundObject = std::shared_ptr<GameObject>(new GameObject());
		fallSoundObject->SetName("FallSound");
		m_fallSound = fallSoundObject->AddComponent<SoundClip>(fallSoundObject);
		m_fallSound->LoadWavSound(L"res/sound/fall.wav");

		// �v���C���[�̎q�I�u�W�F�N�g�ɒǉ�
		m_owner.lock()->AddChild(jumpSoundObject);
		m_owner.lock()->AddChild(damageSoundObject);
		m_owner.lock()->AddChild(fallSoundObject);
	}
	void Player::Start()
	{
		MusicPlayer* musicPlayer = GameObjectManager::FindObject("MusicPlayer")->GetComponent<MusicPlayer>();
		float bpm = musicPlayer->GetBPM();
		float beat = musicPlayer->GetBeat();

		// 4�������̊Ԋu���v�Z
		float barTimeLength = 60.f / bpm * beat;
		m_gameReadyAnimationInterval = barTimeLength / 4.f;

		// �W�����v�͐ݒ�
		m_jumpVelocity = -550.f * 120.f / bpm;
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
	void Player::DrawInspector()
	{
		if (ImGui::CollapsingHeader("Player"))
		{
			ImGui::InputFloat("JumpVelocity", &m_jumpVelocity);
		}
	}
	void Player::OnCollision(Framework::Collider* other)
	{
		// ����������Q���ɓ���������Q�[���I�[�o�[
		if (m_isAlive && other->GetOwner()->GetTag() == "Obstacle")
		{
			OnDead();
		}
	}
	void Player::ChangeSprite()
	{
		// �������͗����X�v���C�g��\��
		if (m_rigidbody->velocity.y >= 0.f)
		{
			m_spriteRenderer->ChangeRenderSprite(static_cast<size_t>(STATE::FALL));
		}
		// �㏸���͏㏸�X�v���C�g��\��
		else
		{
			m_spriteRenderer->ChangeRenderSprite(static_cast<size_t>(STATE::JUMP));
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
		m_rigidbody->velocity = { 0.f, 0.f };
		m_rigidbody->AddForce({ 0.f, m_jumpVelocity }, FORCE_MODE::VELOCITY);
		m_jumpSound->Play(0.1f);
	}

	void Player::LimitPosition()
	{
		// �v���C���[�̈ړ�����
		auto& viewport = Dx12GraphicsEngine::GetViewport();

		// ��[����
		if (m_transform->position.y < 0.f)
		{
			m_transform->position.y = 0.f;
			m_rigidbody->velocity = { 0.f, 0.f };
		}
		// ���[����
		else if (m_transform->position.y > viewport.Height)
		{
			m_transform->position.y = viewport.Height;
			m_rigidbody->velocity = { 0.f, 0.f };
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
		m_spriteRenderer->ChangeRenderSprite(static_cast<size_t>(STATE::DEAD));

		m_transform->position.y += 2.5f;
		m_transform->angle += 2.5f;
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
		m_rigidbody->SetActive(false);

		// �_���[�W�����Đ�
		m_damageSound->Play(0.1f, 0.f, true);

		// ���������Đ�
		m_fallSound->Play(0.1f);
	}
}