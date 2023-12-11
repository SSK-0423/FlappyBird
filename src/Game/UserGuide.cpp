#include "pch.h"
#include "UserGuide.h"
#include "GameScene.h"

using namespace Framework;

namespace FlappyBird
{
	UserGuide::UserGuide(Framework::Object* owner) :
		Framework::IComponent(owner), m_elapsedTime(0.f), m_animationInverval(0.5f), m_currentSpriteIndex(0)
	{
		m_mouseSprites.resize(2);
		m_mouseSprites[0] = std::make_shared<Sprite>(L"res/texture/mouse.png");
		m_mouseSprites[1] = std::make_shared<Sprite>(L"res/texture/mouse_leftclick.png");

		auto windowSize = Window::GetWindowSize();

		SpriteRenderer* spriteRenderer = m_owner->AddComponent<SpriteRenderer>(m_owner);
		spriteRenderer->SetSprite(m_mouseSprites[m_currentSpriteIndex]);
		spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GUI);
		spriteRenderer->SetLayer(static_cast<UINT>(GAME_SCENE_LAYER::UI));

		Transform2D* transform = m_owner->GetComponent<Transform2D>();
		transform->position = { windowSize.cx / 2.f + 100.f, windowSize.cy / 2.f };
		transform->scale = { 100.f, 100.f };
	}

	void UserGuide::Update(float deltaTime)
	{
		// 一定間隔で画像を切り替えてマウスクリックアニメーションを再生
		m_elapsedTime += deltaTime;
		if (m_elapsedTime >= m_animationInverval)
		{
			m_elapsedTime = 0.f;
			m_owner->GetComponent<SpriteRenderer>()->SetSprite(m_mouseSprites[m_currentSpriteIndex]);
			m_currentSpriteIndex = (m_currentSpriteIndex + 1) % m_mouseSprites.size();
		}
	}

	void UserGuide::Draw()
	{
	}
}