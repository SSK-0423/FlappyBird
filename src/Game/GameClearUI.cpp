#include "pch.h"
#include "GameClearUI.h"
#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace Framework;
using namespace DX12Wrapper;

namespace FlappyBird
{
	GameClearUI::GameClearUI(std::shared_ptr<Framework::Object> owner) : IComponent(owner)
	{
		m_owner.lock()->SetName("GameClearUI");

		auto& viewportSize = Dx12GraphicsEngine::GetViewport();

		// �Q�[���N���AUI�̉摜��ǉ�

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
		Text* gameClearText = gameOver->AddComponent<Text>(gameOver);
		gameClearText->SetText(L"Game Clear");
		gameClearText->SetPosition({ viewportSize.Width / 2.f - 220.f, viewportSize.Height / 2.f - 150.f });
		gameClearText->SetScale(0.75f);
		gameClearText->SetColor(DirectX::Colors::Yellow);
		m_owner.lock()->AddChild(gameOver);

		// �����ŃX�R�A�Ƃ��\������


		m_owner.lock()->SetActive(false);
	}
	void GameClearUI::Update(float deltaTime)
	{
	}
	void GameClearUI::Draw()
	{
	}
}