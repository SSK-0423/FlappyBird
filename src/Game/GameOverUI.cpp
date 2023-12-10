#include "pch.h"
#include "GameOverUI.h"

using namespace Framework;

namespace FlappyBird
{
	GameOverUI::GameOverUI(Framework::Object* owner) :
		IComponent(owner)
	{
		m_owner->SetName("GameOverUI");

		auto windowSize = Window::GetWindowSize();

		// ゲームオーバーUIの画像を追加

		// 背景
		//UIObject* background = UIObjectManager::CreateObject();
		//Sprite* backgroundSprite = background->AddComponent<Sprite>(background);
		//backgroundSprite->SetTexture(L"res/texture/GameOverBackground.png");

		// リトライボタン
		UIObject* retry = UIObjectManager::CreateObject();
		Button* retryButton = retry->AddComponent<Button>(retry);
		retryButton->SetTexture(L"res/texture/RetryButton.png");
		retryButton->SetPosition({ windowSize.cx / 2.f, windowSize.cy / 2.f });
		retryButton->SetScale({ 200.f, 50.f });
		retryButton->SetOnClick([]() { SceneManager::SetNextScene("Game"); });
		m_owner->AddChild(retry);
		
		// 辞めるボタン
		UIObject* title = UIObjectManager::CreateObject();
		Button* titleButton = title->AddComponent<Button>(title);
		titleButton->SetTexture(L"res/texture/QuitButton.png");
		titleButton->SetPosition({ windowSize.cx / 2.f, windowSize.cy / 2.f + 150.f });
		titleButton->SetScale({ 200.f, 50.f });
		titleButton->SetOnClick([]() { SceneManager::SetNextScene("Title"); });
		m_owner->AddChild(title);

		// テキスト
		UIObject* gameOver = UIObjectManager::CreateObject();
		Text* gameOverText = gameOver->AddComponent<Text>(gameOver);
		gameOverText->SetText(L"Game Over");
		gameOverText->SetPosition({ windowSize.cx / 2.f, windowSize.cy / 2.f - 150.f });
		gameOverText->SetScale(0.5f);
		gameOverText->SetColor(DirectX::Colors::AliceBlue);
		m_owner->AddChild(gameOver);
	}
	void GameOverUI::Update(float deltaTime)
	{
	}
	void GameOverUI::Draw()
	{
	}
}
