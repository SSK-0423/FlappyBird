#include "pch.h"
#include "Background.h"

#include "Framework/GameFramework.h"

#include "GameScene.h"

using namespace Framework;

namespace FlappyBird
{
	Background::Background(Framework::Object* owner)
		: IComponent(owner)
	{
		auto size = Window::GetWindowSize();

		Sprite* sprite = new Sprite(L"res/texture/background.jpg");
		SpriteRenderer* spriteRenderer = m_owner->AddComponent<SpriteRenderer>(m_owner);
		spriteRenderer->SetSprite(sprite);
		spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GUI);
		spriteRenderer->SetLayer(static_cast<UINT>(GAME_SCENE_LAYER::BACKGROUND));

		Transform2D* transform = m_owner->GetComponent<Transform2D>();
		transform->position = { size.cx / 2.f, size.cy / 2.f };
		transform->scale = { size.cx * 1.f, size.cy * 1.f };
	}
	Background::~Background()
	{
	}
	void Background::Update(float deltaTime)
	{
	}
	void Background::Draw()
	{
	}
}
