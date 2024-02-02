#include "pch.h"
#include "BarLine.h"
#include "FlappyBird.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace DX12Wrapper;
using namespace Framework;

namespace FlappyBird
{
	BarLine::BarLine(Framework::Object* owner) : IComponent(owner)
	{
		// スプライト追加
		std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(L"bar_line.png");
		SpriteRenderer* spriteRenderer = owner->AddComponent<SpriteRenderer>(m_owner);
		spriteRenderer->SetSprite(sprite);
		spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GUI);
		spriteRenderer->SetLayer(SPRITE_LAYER::UI);

		// テスト用のTransform2D
		auto viewportSize = Dx12GraphicsEngine::GetViewport();
		Transform2D* transform = owner->GetComponent<Transform2D>();
		transform->position = { 0.0f, 0.0f };
		transform->scale = { viewportSize.Width,viewportSize.Height };
	}
	BarLine::~BarLine()
	{
	}
	void BarLine::Update(float deltaTime)
	{
	}
	void BarLine::Draw()
	{
	}
}