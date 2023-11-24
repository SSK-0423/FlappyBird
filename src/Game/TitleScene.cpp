#include "pch.h"
#include "TitleScene.h"

#include "Framework/Window.h"

#include "Framework/UIObjectManager.h"

#include "Framework/Sprite.h"
#include "Framework/SpriteRenderer.h"
#include "Framework/Text.h"
#include "Framework/Transform2D.h"

#include "PressEnterText.h"

using namespace Framework;

namespace FlappyBird
{
	void TitleScene::Init()
	{
		auto size = Window::GetWindowSize();

		// 背景
		UIObject* background = UIObjectManager::CreateObject();
		Sprite* backgroundSprite = new Sprite(L"res/Background.jpg");
		background->AddComponent<SpriteRenderer>(background);
		background->GetComponent<SpriteRenderer>()->SetSprite(backgroundSprite);
		background->GetComponent<SpriteRenderer>()->SetDrawMode(SPRITE_DRAW_MODE::GUI);
		background->GetComponent<Transform2D>()->position = { size.cx / 2.f, size.cy / 2.f };
		background->GetComponent<Transform2D>()->scale = { size.cx * 1.f, size.cy * 1.f };

		// タイトルテキストの輪郭線
		UIObject* text = UIObjectManager::CreateObject();
		text->AddComponent<Text>(text);
		text->GetComponent<Text>()->SetText(L"Flappy Bird");
		text->GetComponent<Text>()->SetPosition({ size.cx / 6.f, size.cy / 8.f });
		text->GetComponent<Text>()->SetScale(1.005f);
		text->GetComponent<Text>()->SetColor(DirectX::Colors::White);

		// タイトルテキスト
		UIObject* titleText = UIObjectManager::CreateObject();
		titleText->AddComponent<Text>(titleText);
		titleText->GetComponent<Text>()->SetText(L"Flappy Bird");
		titleText->GetComponent<Text>()->SetPosition({ size.cx / 6.f, size.cy / 8.f });
		titleText->GetComponent<Text>()->SetScale(1.f);
		titleText->GetComponent<Text>()->SetColor(DirectX::Colors::GreenYellow);

		// スタートテキスト
		UIObject* pressEnterText = UIObjectManager::CreateObject();
		pressEnterText->AddComponent<PressEnterText>(pressEnterText);

		OutputDebugStringA("TitleScene Init\n");
	}
	void TitleScene::Final()
	{
		Scene::Final();

		OutputDebugStringA("TitleScene Final\n");
	}
}