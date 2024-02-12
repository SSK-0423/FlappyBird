#include "pch.h"
#include "UserGuide.h"
#include "GameScene.h"
#include "FlappyBird.h"
#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace Framework;
using namespace DX12Wrapper;

namespace FlappyBird
{
	UserGuide::UserGuide(std::shared_ptr<Framework::Object> owner) :
		Framework::IComponent(owner), m_elapsedTime(0.f), m_animationInverval(0.65f), m_currentSpriteIndex(0)
	{
		m_mouseSprites.resize(2);
		m_mouseSprites[0] = std::make_shared<Sprite>(L"res/texture/mouse.png");
		m_mouseSprites[1] = std::make_shared<Sprite>(L"res/texture/mouse_leftclick.png");

		auto viewportSize = Dx12GraphicsEngine::GetViewport();

		SpriteRenderer* spriteRenderer = m_owner.lock()->AddComponent<SpriteRenderer>(m_owner.lock());
		spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GUI);
		spriteRenderer->SetLayer(static_cast<UINT>(SPRITE_LAYER::UI));
		
		for (auto& sprite : m_mouseSprites)
		{
			spriteRenderer->AddSprite(sprite);
		}

		Transform2D* transform = m_owner.lock()->GetComponent<Transform2D>();
		transform->position = { viewportSize.Width / 2.f + 100.f, viewportSize.Height / 2.f };
		transform->scale = { 100.f, 100.f };
	}

	void UserGuide::Update(float deltaTime)
	{
		// 一定間隔で画像を切り替えてマウスクリックアニメーションを再生
		m_elapsedTime += deltaTime;
		if (m_elapsedTime >= m_animationInverval)
		{
			m_elapsedTime = 0.f;
			m_owner.lock()->GetComponent<SpriteRenderer>()->ChangeRenderSprite(m_currentSpriteIndex);
			m_currentSpriteIndex = (m_currentSpriteIndex + 1) % m_mouseSprites.size();
		}
	}

	void UserGuide::Draw()
	{
	}
}