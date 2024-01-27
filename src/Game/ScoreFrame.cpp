#include "pch.h"
#include "ScoreFrame.h"
#include "GameScene.h"
#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace Framework;
using namespace DX12Wrapper;

namespace FlappyBird
{
	ScoreFrame::ScoreFrame(Framework::Object* owner) :
		Framework::IComponent(owner)
	{
		auto viewportSize = Dx12GraphicsEngine::GetViewport();

		// �X�v���C�g�ǉ�
		Sprite* sprite = new Sprite(L"res/texture/score_frame.png", SPRITE_PIVOT::TOP_LEFT);
		SpriteRenderer* spriteRenderer = m_owner->AddComponent<SpriteRenderer>(m_owner);
		spriteRenderer->SetSprite(sprite);
		spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GUI);
		spriteRenderer->SetLayer(static_cast<UINT>(GAME_SCENE_LAYER::UI));

		Transform2D* transform = m_owner->GetComponent<Transform2D>();
		transform->position = { viewportSize.Width / 2.0f - 75.f, 0.f };
		transform->scale = { 350.f, 50.f };
	}

	void ScoreFrame::Update(float deltaTime)
	{
	}

	void ScoreFrame::Draw()
	{
	}
}