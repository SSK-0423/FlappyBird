#include "pch.h"
#include "JudgeEffect.h"

#include "FlappyBird.h"

using namespace Framework;
namespace FlappyBird
{
	// 静的メンバ変数の実体化
	float JudgeEffect::m_riseSpeed = 2.5f;
	float JudgeEffect::m_lifeTime = 0.5f;
	float JudgeEffect::m_initPosY = 400.f;

	JudgeEffect::JudgeEffect(std::shared_ptr<Framework::Object> owner) : IComponent(owner)
	{
		m_transform = m_owner.lock()->GetComponent<Transform2D>();
		m_transform->position = { 100.f, m_initPosY };

		//Sprite* sprite = new Sprite(L"res/texture/judge_effect.png");
		//SpriteRenderer* spriteRenderer = m_owner.lock()->AddComponent<SpriteRenderer>(m_owner.lock());
		//spriteRenderer->SetSprite(sprite);
		//spriteRenderer->SetLayer(SPRITE_LAYER::UI);
		//spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GUI);

		m_judgeText = m_owner.lock()->AddComponent<Text>(m_owner.lock());
		m_judgeText->SetScale(0.3f);
	}
	JudgeEffect::~JudgeEffect()
	{
	}
	void JudgeEffect::Update(float deltaTime)
	{
		m_timer += deltaTime;

		// エフェクトの寿命をチェック
		CheckLifeTime();

		// 上昇アニメーション
		RiseAnimation();
	}
	void JudgeEffect::Draw()
	{
	}
	void JudgeEffect::SetJudgeText(const std::wstring& text)
	{
		m_judgeText->SetText(text);
	}
	void JudgeEffect::SetJudgeTextColor(const DirectX::XMVECTORF32& color)
	{
		m_judgeText->SetColor(color);
	}
	void JudgeEffect::CheckLifeTime()
	{
		if (m_timer > m_lifeTime)
		{
			m_owner.lock()->SetActive(false);
			m_timer = 0.0f;
			m_transform->position.y = m_initPosY;
		}
	}
	void JudgeEffect::RiseAnimation()
	{
		m_transform->position.y -= m_riseSpeed;
	}
}