#include "pch.h"
#include "TitleScene.h"

#include "Framework/Window.h"

#include "Framework/UIObjectManager.h"

#include "Framework/Sprite.h"
#include "Framework/SpriteRenderer.h"
#include "Framework/Text.h"
#include "Framework/Transform2D.h"

#include "PleaseClickText.h"

using namespace Framework;

namespace FlappyBird
{
	void TitleScene::Init()
	{
		auto size = Window::GetWindowSize();

		// 背景
		UIObject* background = UIObjectManager::CreateObject();
		Sprite* backgroundSprite = new Sprite(L"res/texture/Background.jpg");
		SpriteRenderer* backgroundRenderer = background->AddComponent<SpriteRenderer>(background);
		backgroundRenderer->SetSprite(backgroundSprite);
		backgroundRenderer->SetDrawMode(SPRITE_DRAW_MODE::GUI);
		Transform2D* backgroundTransform = background->GetComponent<Transform2D>();
		backgroundTransform->position = { size.cx / 2.f, size.cy / 2.f };
		backgroundTransform->scale = { size.cx * 1.f, size.cy * 1.f };

		// タイトルテキストの輪郭線
		UIObject* outline = UIObjectManager::CreateObject();
		Text* outlineText = outline->AddComponent<Text>(outline);
		outlineText->SetText(L"Flappy Bird");
		outlineText->SetPosition({ size.cx / 6.f, size.cy / 8.f });
		outlineText->SetScale(1.005f);
		outlineText->SetColor(DirectX::Colors::White);

		// タイトルテキスト
		UIObject* title = UIObjectManager::CreateObject();
		Text* titleText = title->AddComponent<Text>(title);
		titleText->SetText(L"Flappy Bird");
		titleText->SetPosition({ size.cx / 6.f, size.cy / 8.f });
		titleText->SetScale(1.f);
		titleText->SetColor(DirectX::Colors::Green);

		// スタートテキスト
		UIObject* pleaseClick = UIObjectManager::CreateObject();
		pleaseClick->AddComponent<PleaseClickText>(pleaseClick);

		OutputDebugStringA("TitleScene Init\n");
	}
	void TitleScene::Final()
	{
		Scene::Final();

		OutputDebugStringA("TitleScene Final\n");
	}
}