#include "pch.h"
#include "GameOverUI.h"
#include "GameScene.h"
#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace Framework;
using namespace DX12Wrapper;

namespace FlappyBird
{
	GameOverUI::GameOverUI(std::shared_ptr<Framework::Object> owner) :
		IComponent(owner)
	{
		m_owner.lock()->SetName("GameOverUI");

		auto viewportSize = Dx12GraphicsEngine::GetViewport();

		// �Q�[���I�[�o�[UI�̉摜��ǉ�

		// ���g���C�{�^��
		std::shared_ptr<Framework::UIObject> retry = UIObjectManager::CreateObject();
		Button* retryButton = retry->AddComponent<Button>(retry);
		retryButton->SetTexture(L"res/texture/retry_button.png");
		retryButton->SetPosition({ viewportSize.Width / 2.f, viewportSize.Height / 2.f });
		retryButton->SetScale({ 300.f, 300.f / 4.f });
		retryButton->SetOnClick([]() { SceneManager::SetNextScene("Game"); });
		m_owner.lock()->AddChild(retry);

		// ���߂�{�^��
		std::shared_ptr<Framework::UIObject> title = UIObjectManager::CreateObject();
		Button* titleButton = title->AddComponent<Button>(title);
		titleButton->SetTexture(L"res/texture/quit_button.png");
		titleButton->SetPosition({ viewportSize.Width / 2.f, viewportSize.Height / 2.f + 150.f });
		titleButton->SetScale({ 300.f, 300.f / 4.f });
		titleButton->SetOnClick([]() { SceneManager::SetNextScene("Title"); });
		m_owner.lock()->AddChild(title);

		// �e�L�X�g
		std::shared_ptr<Framework::UIObject> gameOver = UIObjectManager::CreateObject();
		Text* gameOverText = gameOver->AddComponent<Text>(gameOver);
		gameOverText->SetText(L"Game Over");
		gameOverText->SetPosition({ viewportSize.Width / 2.f - 220.f, viewportSize.Height / 2.f - 150.f });
		gameOverText->SetScale(0.75f);
		gameOverText->SetColor(DirectX::Colors::Red);
		m_owner.lock()->AddChild(gameOver);

		m_owner.lock()->SetActive(false);
	}
	void GameOverUI::Update(float deltaTime)
	{
	}
	void GameOverUI::Draw()
	{
	}
}
