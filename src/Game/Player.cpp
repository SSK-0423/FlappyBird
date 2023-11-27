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
		// �X�v���C�g�ǉ�
		Sprite* sprite = new Sprite(L"res/tile_64.png");
		m_owner->AddComponent<SpriteRenderer>(m_owner);
		m_owner->GetComponent<SpriteRenderer>()->SetSprite(sprite);
		m_owner->GetComponent<SpriteRenderer>()->SetDrawMode(SPRITE_DRAW_MODE::GAMEOBJECT);
		m_owner->GetComponent<SpriteRenderer>()->SetLayer(static_cast<UINT>(GAME_SCENE_LAYER::GAMEOBJECT));

		// �R���C�_�[�ǉ�
		RectCollider* collider = m_owner->AddComponent<RectCollider>(m_owner);
		collider->SetRectSize(100.f, 100.f);

		// ���W�b�h�{�f�B�ǉ�
		Rigidbody2D* rigidbody = m_owner->AddComponent<Rigidbody2D>(m_owner);
		rigidbody->gravityScale = 1.5f;

		// �v���C���[�̈ʒu��ݒ�
		auto windowSize = Window::GetWindowSize();
		m_owner->GetComponent<Transform2D>()->position = { 200.f, windowSize.cy / 2.f };
	}
	Player::~Player()
	{
	}
	void Player::Update(float deltaTime)
	{
		// �}�E�X���N���b�N�ŃW�����v
		if (InputSystem::GetMouseButtonDown(MOUSECODE::LEFT))
		{
			// ���㌴�_�Ȃ̂ŁA�������ɗ͂�������
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