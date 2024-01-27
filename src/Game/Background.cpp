#include "pch.h"
#include "Background.h"

#include "Framework/GameFramework.h"

#include "GameScene.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace Framework;
using namespace DX12Wrapper;

namespace FlappyBird
{
	Background::Background(Framework::Object* owner)
		: IComponent(owner)
	{
		auto viewportSize = Dx12GraphicsEngine::GetViewport();

		Sprite* sprite = new Sprite(L"res/texture/sky.png");
		SpriteRenderer* spriteRenderer = m_owner->AddComponent<SpriteRenderer>(m_owner);
		spriteRenderer->SetSprite(sprite);
		spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GUI);
		spriteRenderer->SetLayer(static_cast<UINT>(GAME_SCENE_LAYER::BACKGROUND));

		Transform2D* transform = m_owner->GetComponent<Transform2D>();
		transform->position = { viewportSize.Width / 2.f, viewportSize.Height / 2.f };
		transform->scale = { viewportSize.Width * 1.f, viewportSize.Height * 1.f };
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
