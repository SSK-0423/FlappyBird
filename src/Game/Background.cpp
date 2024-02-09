#include "pch.h"
#include "Background.h"

#include "Framework/GameFramework.h"

#include "GameScene.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

#include "FlappyBird.h"

using namespace Framework;
using namespace DX12Wrapper;

namespace FlappyBird
{
	Background::Background(std::shared_ptr<Framework::Object> owner)
		: IComponent(owner)
	{
		m_owner.lock()->SetName("Background");

		auto viewportSize = Dx12GraphicsEngine::GetViewport();

		Sprite* sprite = new Sprite(L"res/texture/sky.png");
		SpriteRenderer* spriteRenderer = m_owner.lock()->AddComponent<SpriteRenderer>(m_owner.lock());
		spriteRenderer->SetSprite(sprite);
		spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GUI);
		spriteRenderer->SetLayer(static_cast<UINT>(SPRITE_LAYER::BACKGROUND));

		Transform2D* transform = m_owner.lock()->GetComponent<Transform2D>();
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
