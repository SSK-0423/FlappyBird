#include "pch.h"
#include "NotesEditScene.h"
#include "NotesEditUI.h"
#include "NotesEditor.h"

#include "BarManager.h"
#include "NotesManager.h"
#include "MusicPlayer.h"

#include "FlappyBird.h"

#include "../Framework/GameFramework.h"
#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace Framework;
using namespace DX12Wrapper;

namespace FlappyBird
{
	void NotesEditScene::Init()
	{
		// TODO: ここに初期化処理を書く
#ifdef _DEBUG
		Editor::DebugLog("NotesEditScene::Init");
#endif // _DEBUG

		auto viewportSize = Dx12GraphicsEngine::GetViewport();

		// 背景
		std::shared_ptr<GameObject> background = GameObjectManager::CreateObject();
		background->SetName("Background");
		Sprite* backgroundSprite = new Sprite(L"res/texture/sky.png");
		SpriteRenderer* backgroundRenderer = background->AddComponent<SpriteRenderer>(background);
		backgroundRenderer->SetSprite(backgroundSprite);
		backgroundRenderer->SetDrawMode(SPRITE_DRAW_MODE::GUI);
		backgroundRenderer->SetLayer(static_cast<UINT>(SPRITE_LAYER::BACKGROUND));
		Transform2D* backgroundTransform = background->GetComponent<Transform2D>();
		backgroundTransform->position = { viewportSize.Width / 2.f, viewportSize.Height / 2.f };
		backgroundTransform->scale = { viewportSize.Width * 1.f, viewportSize.Height * 1.f };

		// タイトル画面へ戻るボタン
		std::shared_ptr<UIObject> backToTitleButton = UIObjectManager::CreateObject();
		backToTitleButton->SetName("BackToTitleButton");
		Button* backButton = backToTitleButton->AddComponent<Button>(backToTitleButton);
		backButton->SetTexture(L"res/texture/back_to_title_button.png");
		backButton->SetPosition({ viewportSize.Width - 100.f, 25.f });
		backButton->SetScale({ 200.f, 200.f / 4.f });
		backButton->SetLayer(static_cast<UINT>(SPRITE_LAYER::UI));
		backButton->SetOnClick([]() {SceneManager::SetNextScene("Title"); });

		// ノーツ編集関連のUI
		std::shared_ptr<UIObject> editWindow = UIObjectManager::CreateObject();
		editWindow->SetName("NotesEditUI");
		editWindow->AddComponent<NotesEditUI>(editWindow);

		// 小節線の管理オブジェクト
		std::shared_ptr<UIObject> barManagerObj = UIObjectManager::CreateObject();
		barManagerObj->SetName("BarManager");
		BarManager* barManager = barManagerObj->AddComponent<BarManager>(barManagerObj);

		// 判定ラインの生成
		std::shared_ptr<UIObject> judgeLineObj = UIObjectManager::CreateObject();
		judgeLineObj->SetName("JudgeLine");
		Sprite* judgeLineSprite = new Sprite(L"res/texture/judge_line.png");
		SpriteRenderer* judgeLineRenderer = judgeLineObj->AddComponent<SpriteRenderer>(judgeLineObj);
		judgeLineRenderer->SetSprite(judgeLineSprite);
		judgeLineRenderer->SetDrawMode(SPRITE_DRAW_MODE::GUI);
		judgeLineRenderer->SetLayer(static_cast<UINT>(SPRITE_LAYER::UI));
		Transform2D* judgeLineTransform = judgeLineObj->GetComponent<Transform2D>();
		judgeLineTransform->position = { 200.f, viewportSize.Height / 2.f };
		judgeLineTransform->scale = { 5.f, viewportSize.Height };

		// ノーツの管理オブジェクト
		std::shared_ptr<GameObject> notesManagerObj = GameObjectManager::CreateObject();
		notesManagerObj->SetName("NotesManager");
		NotesManager* notesManager = notesManagerObj->AddComponent<NotesManager>(notesManagerObj);

		// 曲再生管理オブジェクト
		std::shared_ptr<GameObject> musicPlayerObj = GameObjectManager::CreateObject();
		musicPlayerObj->SetName("MusicPlayer");
		MusicPlayer* musicPlayer = musicPlayerObj->AddComponent<MusicPlayer>(musicPlayerObj);

		// ノーツエディターの管理オブジェクト
		std::shared_ptr<GameObject> notesEditorObj = GameObjectManager::CreateObject();
		notesEditorObj->SetName("NotesEditor");
		notesEditorObj->AddComponent<NotesEditor>(notesEditorObj);
	}

	void NotesEditScene::Final()
	{
#ifdef _DEBUG
		Editor::DebugLog("NotesEditScene::Final");
#endif // _DEBUG
	}
}

