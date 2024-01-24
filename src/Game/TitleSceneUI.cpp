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
		singlePlay->SetName("GameStartButton");
		Button* singlePlayButton = singlePlay->AddComponent<Button>(singlePlay);
		singlePlayButton->SetTexture(L"res/texture/game_start_button.png");
		singlePlayButton->SetPosition({ windowSize.cx / 2.f, windowSize.cy / 2.f });
		singlePlayButton->SetScale({ 300.f, 300.f / 4.f });
		singlePlayButton->SetLayer(static_cast<UINT>(TITLE_SCENE_LAYER::UI));
		singlePlayButton->SetOnClick([]() {SceneManager::SetNextScene("Game"); });
		m_gameModeButtons.push_back(singlePlay);

		// ノーツエディットボタン
		UIObject* notesEdit = UIObjectManager::CreateObject();
		notesEdit->SetName("NotesEditButton");
		Button* notesEditButton = notesEdit->AddComponent<Button>(notesEdit);
		notesEditButton->SetTexture(L"res/texture/notes_edit_button.png");
		notesEditButton->SetPosition({ windowSize.cx / 2.f, windowSize.cy / 2.f + 100.f });
		notesEditButton->SetScale({ 300.f, 300.f / 4.f });
		notesEditButton->SetLayer(static_cast<UINT>(TITLE_SCENE_LAYER::UI));
		notesEditButton->SetOnClick([]() {SceneManager::SetNextScene("NotesEdit"); });
		m_gameModeButtons.push_back(notesEdit);

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
