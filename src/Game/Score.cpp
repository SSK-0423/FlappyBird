#include "pch.h"
#include "Score.h"

using namespace Framework;

namespace FlappyBird
{
	Score::Score(Object* owner) :
		IComponent(owner), m_score(0), m_scoreAddInterval(1.0f), m_elapsedTime(0.f)
	{
		auto windowSize = Window::GetWindowSize();

		// テキスト追加
		Text* text = m_owner->AddComponent<Text>(m_owner);
		text->SetText(L"SCORE");
		text->SetScale(0.25f);
		text->SetPosition({ windowSize.cx / 2.0f - 50.0f, 0.f });
		text->SetColor(DirectX::Colors::Black);

		m_player = GameObjectManager::FindObject("Player")->GetComponent<Player>();
	}

	void Score::Update(float deltaTime)
	{
		// プレイヤーが死んでいたら更新しない
		if (m_player->IsDead())
		{
			return;
		}

		// 一定時間間隔でスコアを加算
		m_elapsedTime += deltaTime;
		if (m_elapsedTime >= m_scoreAddInterval)
		{
			AddScore(1);
			m_elapsedTime = 0.f;
		}
		m_owner->GetComponent<Text>()->SetText(L"SCORE: " + std::to_wstring(m_score));
	}

	void Score::Draw()
	{
	}

	void Score::AddScore(unsigned int score)
	{
		m_score += score;
	}

	unsigned int Score::GetScore()
	{
		return m_score;
	}
}