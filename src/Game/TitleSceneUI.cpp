#include "pch.h"
#include "TitleSceneUI.h"
#include "TitleScene.h"

#include "FlappyBird.h"
#include "PleaseClickText.h"
#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace Framework;

namespace FlappyBird
{
	TitleSceneUI::TitleSceneUI(std::shared_ptr<Object> owner) : IComponent(owner)
	{
		m_owner.lock()->SetName("TitleSceneUI");

		auto viewport = DX12Wrapper::Dx12GraphicsEngine::GetViewport();
		auto scissorRect = DX12Wrapper::Dx12GraphicsEngine::GetScissorRect();
		SIZE windowSize;
		windowSize.cx = static_cast<LONG>(viewport.Width);
		windowSize.cy = static_cast<LONG>(viewport.Height);

		Editor::DebugLog("Window Size: %d, %d", windowSize.cx, windowSize.cy);

		// タイトルテキストの輪郭線
		std::shared_ptr<UIObject> outline = UIObjectManager::CreateObject();
		Text* outlineText = outline->AddComponent<Text>(outline);
		outlineText->SetText(L"Flappy Bird");
		outlineText->SetPosition({ windowSize.cx / 6.f + 5.f, windowSize.cy / 7.f + 5.f });
		outlineText->SetScale(1.f);
		outlineText->SetColor(DirectX::Colors::White);

		// タイトルテキスト
		std::shared_ptr<UIObject> title = UIObjectManager::CreateObject();
		Text* titleText = title->AddComponent<Text>(title);
		titleText->SetText(L"Flappy Bird");
		titleText->SetPosition({ windowSize.cx / 6.f, windowSize.cy / 7.f });
		titleText->SetScale(1.f);
		titleText->SetColor(DirectX::Colors::Green);

		// スタートテキスト
		std::shared_ptr<UIObject> pleaseClick = UIObjectManager::CreateObject();
		PleaseClickText* pleaseClickText = pleaseClick->AddComponent<PleaseClickText>(pleaseClick);
		pleaseClickText->OnClicked.Subscribe(std::bind(&TitleSceneUI::ShowGameModeButtons, this, std::placeholders::_1));

		// ゲームモード選択ボタン
		// ゲームスタートボタン
		std::shared_ptr<UIObject> singlePlay = UIObjectManager::CreateObject();
		singlePlay->SetName("GameStartButton");
		Button* singlePlayButton = singlePlay->AddComponent<Button>(singlePlay);
		singlePlayButton->SetTexture(L"res/texture/game_start_button.png");
		singlePlayButton->SetPosition({ windowSize.cx / 2.f, windowSize.cy / 2.f });
		singlePlayButton->SetScale({ 300.f, 300.f / 4.f });
		singlePlayButton->SetLayer(static_cast<UINT>(SPRITE_LAYER::UI));
		singlePlayButton->SetOnClick([]() {SceneManager::SetNextScene("MusicSelect"); });
		m_gameModeButtons.push_back(singlePlay);

		// ノーツエディットボタン
		std::shared_ptr<UIObject> notesEdit = UIObjectManager::CreateObject();
		notesEdit->SetName("NotesEditButton");
		Button* notesEditButton = notesEdit->AddComponent<Button>(notesEdit);
		notesEditButton->SetTexture(L"res/texture/notes_edit_button.png");
		notesEditButton->SetPosition({ windowSize.cx / 2.f, windowSize.cy / 2.f + 100.f });
		notesEditButton->SetScale({ 300.f, 300.f / 4.f });
		notesEditButton->SetLayer(static_cast<UINT>(SPRITE_LAYER::UI));
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
	}

	void TitleSceneUI::Draw()
	{
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
