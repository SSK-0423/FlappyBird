#include "pch.h"
#include "TestObstacle.h"

#include "Framework/GameFramework.h"

using namespace Framework;

namespace FlappyBird
{
	TestObstacle::TestObstacle(Object* owner)
		: IComponent(owner)
	{
		m_owner->SetName("TestObstacle");

		// 必要コンポーネント追加
		// スプライト
		Sprite* sprite = new Sprite(L"res/tile_8.png");
		m_owner->AddComponent<SpriteRenderer>(m_owner);
		m_owner->GetComponent<SpriteRenderer>()->SetSprite(sprite);
		m_owner->GetComponent<SpriteRenderer>()->SetDrawMode(SPRITE_DRAW_MODE::GAMEOBJECT);
		m_owner->GetComponent<SpriteRenderer>()->SetLayer(0);
		// コライダー
		m_owner->AddComponent<RectCollider>(m_owner);
		m_owner->GetComponent<RectCollider>()->SetRectSize(100, 100);

		m_owner->GetComponent<Transform2D>()->position.x = Window::GetWindowSize().cx / 2;
		m_owner->GetComponent<Transform2D>()->position.y = Window::GetWindowSize().cy / 2;
	}
	TestObstacle::~TestObstacle()
	{
	}
	void TestObstacle::Update(float deltaTime)
	{
	}
	void TestObstacle::Draw()
	{
	}
}