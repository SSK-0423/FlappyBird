#include "pch.h"
#include "GameOverUI.h"
#include "GameScene.h"
#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace Framework;
using namespace DX12Wrapper;

namespace FlappyBird
{
	GameOverUI::GameOverUI(Framework::Object* owner) :
		IComponent(owner)
	{
		m_owner->SetName("GameOverUI");

		auto viewport = Dx12GraphicsEngine::GetViewport();
		SIZE viewportSize = { viewport.Width, viewport.Height };

		// ゲームオーバーUIの画像を追加

		// リトライボタン
		UIObject* retry = UIObjectManager::CreateObject();
		Button* retryButton = retry->AddComponent<Button>(retry);
		retryButton->SetTexture(L"res/texture/retry_button.png");
		retryButton->SetPosition({ viewportSize.cx / 2.f, viewportSize.cy / 2.f });
		retryButton->SetScale({ 300.f, 300.f / 4.f });
		retryButton->SetOnClick([]() { SceneManager::SetNextScene("Game"); });
		m_owner->AddChild(retry);

		// 辞めるボタン
		UIObject* title = UIObjectManager::CreateObject();
		Button* titleButton = title->AddComponent<Button>(title);
		titleButton->SetTexture(L"res/texture/quit_button.png");
		titleButton->SetPosition({ viewportSize.cx / 2.f, viewportSize.cy / 2.f + 150.f });
		titleButton->SetScale({ 300.f, 300.f / 4.f });
		titleButton->SetOnClick([]() { SceneManager::SetNextScene("Title"); });
		m_owner->AddChild(title);

		// テキスト
		UIObject* gameOver = UIObjectManager::CreateObject();
		Text* gameOverText = gameOver->AddComponent<Text>(gameOver);
		gameOverText->SetText(L"Game Over");
		gameOverText->SetPosition({ viewportSize.cx / 2.f - 220.f, viewportSize.cy / 2.f - 150.f });
		gameOverText->SetScale(0.75f);
		gameOverText->SetColor(DirectX::Colors::Red);
		m_owner->AddChild(gameOver);

		m_owner->SetActive(false);
	}
	void GameOverUI::Update(float deltaTime)
	{
	}
	void GameOverUI::Draw()
	{
	}
}
