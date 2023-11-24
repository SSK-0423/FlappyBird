#include "pch.h"
#include "TestCharacter.h"

#include "Framework/GameFramework.h"

using namespace Framework;

namespace FlappyBird
{
	TestCharacter::TestCharacter(Object* owner)
		: IComponent(owner)
	{
		m_owner->SetName("TestCharacter");

		// 必要コンポーネント追加
		// スプライト
		Sprite* sprite = new Sprite(L"res/tile_64.png");
		m_owner->AddComponent<SpriteRenderer>(m_owner);
		m_owner->GetComponent<SpriteRenderer>()->SetSprite(sprite);
		m_owner->GetComponent<SpriteRenderer>()->SetDrawMode(SPRITE_DRAW_MODE::GAMEOBJECT);
		m_owner->GetComponent<SpriteRenderer>()->SetLayer(0);
		// コライダー
		m_owner->AddComponent<RectCollider>(m_owner);
		m_owner->GetComponent<RectCollider>()->SetRectSize(100, 100);
		m_owner->GetComponent<RectCollider>()->SetOnCollisionCallBack(std::bind(&TestCharacter::OnCollision, this, std::placeholders::_1));
	}
	TestCharacter::~TestCharacter()
	{
	}
	void TestCharacter::Update(float deltaTime)
	{
		// 上下左右に移動
		if (InputSystem::GetKey(DIK_W))
		{
			m_owner->GetComponent<Transform2D>()->position.y -= 1.f;
		}
		if (InputSystem::GetKey(DIK_S))
		{
			m_owner->GetComponent<Transform2D>()->position.y += 1.f;
		}
		if (InputSystem::GetKey(DIK_A))
		{
			m_owner->GetComponent<Transform2D>()->position.x -= 1.f;
		}
		if (InputSystem::GetKey(DIK_D))
		{
			m_owner->GetComponent<Transform2D>()->position.x += 1.f;
		}
	}
	void TestCharacter::Draw()
	{
	}
	void TestCharacter::OnCollision(Collider* other)
	{
		OutputDebugStringA("TestCharacter OnCollision\n");
	}
}

