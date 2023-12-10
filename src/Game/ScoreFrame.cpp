#include "pch.h"
#include "ScoreFrame.h"
#include "GameScene.h"

using namespace Framework;

namespace FlappyBird
{
	ScoreFrame::ScoreFrame(Framework::Object* owner) :
		Framework::IComponent(owner)
	{
		auto windowSize = Window::GetWindowSize();

		// スプライト追加
		Sprite* sprite = new Sprite(L"res/texture/score_frame.png", SPRITE_PIVOT::TOP_LEFT);
		SpriteRenderer* spriteRenderer = m_owner->AddComponent<SpriteRenderer>(m_owner);
		spriteRenderer->SetSprite(sprite);
		spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GUI);
		spriteRenderer->SetLayer(static_cast<UINT>(GAME_SCENE_LAYER::UI));

		Transform2D* transform = m_owner->GetComponent<Transform2D>();
		transform->position = { windowSize.cx / 2.0f - 75.f, 0.f };
		transform->scale = { 250.f, 50.f };
	}

	void ScoreFrame::Update(float deltaTime)
	{
	}

	void ScoreFrame::Draw()
	{
	}
}