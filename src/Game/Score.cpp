#include "pch.h"
#include "Score.h"
#include "GameMaster.h"
#include "GameScene.h"
#include "NotesManager.h"
#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace Framework;
using namespace DX12Wrapper;

namespace FlappyBird
{
	Score::Score(std::shared_ptr<Object> owner) :
		IComponent(owner), m_score(0), m_scoreAddInterval(1.0f), m_elapsedTime(0.f)
	{
		auto viewportSize = Dx12GraphicsEngine::GetViewport();

		// テキスト追加
		Text* text = m_owner.lock()->AddComponent<Text>(m_owner.lock());
		text->SetText(L"SCORE: " + std::to_wstring(m_score));
		text->SetScale(0.4f);
		text->SetPosition({ viewportSize.Width / 2.0f - 50.f, 5.f });
		text->SetColor(DirectX::Colors::Black);

		m_gameMaster = GameObjectManager::FindObject("GameMaster")->GetComponent<GameMaster>();
	}

	void Score::Start()
	{
		// ノーツが判定ラインに到達したときにスコアを加算
		auto notesManager = GameObjectManager::FindObject("NotesManager")->GetComponent<NotesManager>();
		notesManager->OnReachedJudgeLine.Subscribe([this](NotificationEvent e) { AddScore(100); });
	}

	void Score::Update(float deltaTime)
	{
		if (m_gameMaster->GetGameState() == GAME_STATE::PLAYING)
		{
			//// 一定時間間隔でスコアを加算
			//m_elapsedTime += deltaTime;
			//if (m_elapsedTime >= m_scoreAddInterval)
			//{
			//	AddScore(1);
			//	m_elapsedTime = 0.f;
			//}
			m_owner.lock()->GetComponent<Text>()->SetText(L"SCORE: " + std::to_wstring(m_score));
		}
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