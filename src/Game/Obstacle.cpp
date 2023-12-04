#include "pch.h"
#include "Obstacle.h"

#include "GameScene.h"

using namespace Framework;

namespace FlappyBird
{
	Obstacle::Obstacle(Framework::Object* owner)
		: IComponent(owner)
	{
		m_owner->SetName("Obstacle");
		m_owner->SetTag("Obstacle");

		auto windowSize = Window::GetWindowSize();

		Transform2D* transform = m_owner->GetComponent<Transform2D>();
		transform->position = { 500.f, 300.f };
		transform->scale = { 100.f, windowSize.cy / 1.f };

		RectCollider* collider = m_owner->AddComponent<RectCollider>(m_owner);
		collider->SetRectSize(transform->scale.x, transform->scale.y);

		Rigidbody2D* rigidbody = m_owner->AddComponent<Rigidbody2D>(m_owner);
		rigidbody->useGravity = false;

		Sprite* sprite = new Sprite(L"res/dokan_long.png");
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
		transform->position.x += m_moveSpeedX;
		transform->position.y += m_moveSpeedY;
	}
	void Obstacle::Draw()
	{
	}
	void Obstacle::OnCollision(Framework::Collider* other)
	{
	}
	void Obstacle::SetMoveSpeed(float moveSpeedX, float moveSpeedY)
	{
		m_moveSpeedX = moveSpeedX;
		m_moveSpeedY = moveSpeedY;
	}
}
