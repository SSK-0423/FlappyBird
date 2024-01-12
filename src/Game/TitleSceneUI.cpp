#include "pch.h"
#include "TitleSceneUI.h"

#include "PleaseClickText.h"

using namespace Framework;

namespace FlappyBird
{
	TitleSceneUI::TitleSceneUI(Framework::Object* owner) : IComponent(owner)
	{
		m_owner->SetName("TitleSceneUI");

		auto windowSize = Window::GetWindowSize();

		// タイトルテキストの輪郭線
		UIObject* outline = UIObjectManager::CreateObject();
		Text* outlineText = outline->AddComponent<Text>(outline);
		outlineText->SetText(L"Flappy Bird");
		outlineText->SetPosition({ windowSize.cx / 6.f + 5.f, windowSize.cy / 7.f + 5.f });
		outlineText->SetScale(1.f);
		outlineText->SetColor(DirectX::Colors::White);

		// タイトルテキスト
		UIObject* title = UIObjectManager::CreateObject();
		Text* titleText = title->AddComponent<Text>(title);
		titleText->SetText(L"Flappy Bird");
		titleText->SetPosition({ windowSize.cx / 6.f, windowSize.cy / 7.f });
		titleText->SetScale(1.f);
		titleText->SetColor(DirectX::Colors::Green);

		// スタートテキスト
		UIObject* pleaseClick = UIObjectManager::CreateObject();
		PleaseClickText* pleaseClickText = pleaseClick->AddComponent<PleaseClickText>(pleaseClick);
		pleaseClickText->OnClicked.Subscribe([=](NotificationEvent e) {SceneManager::SetNextScene("Game"); });
	}
	void TitleSceneUI::Update(float deltaTime)
	{
	}
	void TitleSceneUI::Draw()
	{
	}
}
