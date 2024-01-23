#include "pch.h"
#include "TitleSceneUI.h"
#include "TitleScene.h"

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
		pleaseClickText->OnClicked.Subscribe(std::bind(&TitleSceneUI::ShowGameModeButtons, this, std::placeholders::_1));

		// ゲームモード選択ボタン
		// 1人プレイボタン
		UIObject* singlePlay = UIObjectManager::CreateObject();
		singlePlay->SetName("SinglePlayButton");
		Button* singlePlayButton = singlePlay->AddComponent<Button>(singlePlay);
		singlePlayButton->SetTexture(L"res/texture/single_play_button.png");
		singlePlayButton->SetPosition({ windowSize.cx / 2.f, windowSize.cy / 2.f });
		singlePlayButton->SetScale({ 300.f, 300.f / 4.f });
		singlePlayButton->SetLayer(static_cast<UINT>(TITLE_SCENE_LAYER::UI));
		singlePlayButton->SetOnClick([]() {SceneManager::SetNextScene("Game"); });
		m_gameModeButtons.push_back(singlePlay);

		// 2人プレイボタン
		UIObject* multiPlay = UIObjectManager::CreateObject();
		multiPlay->SetName("MultiPlayButton");
		Button* multiPlayButton = multiPlay->AddComponent<Button>(multiPlay);
		multiPlayButton->SetTexture(L"res/texture/multi_play_button.png");
		multiPlayButton->SetPosition({ windowSize.cx / 2.f, windowSize.cy / 2.f + 150.f });
		multiPlayButton->SetScale({ 300.f, 300.f / 4.f });
		multiPlayButton->SetLayer(static_cast<UINT>(TITLE_SCENE_LAYER::UI));
		multiPlayButton->SetOnClick([]() {SceneManager::SetNextScene("Game"); });
		m_gameModeButtons.push_back(multiPlay);

		// ゲームモード選択ボタンを非表示にする
		for (auto& button : m_gameModeButtons)
		{
			button->SetActive(false);
		}
	}

	void TitleSceneUI::Update(float deltaTime)
	{
		Editor::DebugLog("TitleSceneUI::Update");
		// FPS
		Editor::DebugLog("FPS: %f", 1.f / deltaTime);
	}

	void TitleSceneUI::Draw()
	{
		Editor::DebugLog("TitleSceneUI::Draw");
	}

	void TitleSceneUI::ShowGameModeButtons(NotificationEvent e)
	{
		// ゲームモード選択ボタンを表示する
		for (auto& button : m_gameModeButtons)
		{
			button->SetActive(true);
		}
	}
}
