#include "pch.h"
#include "NotesEditScene.h"
#include "NotesEditUI.h"
#include "FlappyBird.h"

#include "../Framework/GameFramework.h"
#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace Framework;
using namespace DX12Wrapper;

namespace FlappyBird
{
	void NotesEditScene::Init()
	{
		// TODO: �����ɏ���������������
#ifdef _DEBUG
		Editor::DebugLog("NotesEditScene::Init");
#endif // _DEBUG

		auto viewportSize = Dx12GraphicsEngine::GetViewport();

		// �w�i
		GameObject* background = GameObjectManager::CreateObject();
		background->SetName("Background");
		Sprite* backgroundSprite = new Sprite(L"res/texture/sky.png");
		SpriteRenderer* backgroundRenderer = background->AddComponent<SpriteRenderer>(background);
		backgroundRenderer->SetSprite(backgroundSprite);
		backgroundRenderer->SetDrawMode(SPRITE_DRAW_MODE::GUI);
		backgroundRenderer->SetLayer(static_cast<UINT>(SPRITE_LAYER::BACKGROUND));
		Transform2D* backgroundTransform = background->GetComponent<Transform2D>();
		backgroundTransform->position = { viewportSize.Width / 2.f, viewportSize.Height / 2.f };
		backgroundTransform->scale = { viewportSize.Width * 1.f, viewportSize.Height * 1.f };

		// UI
		UIObject* editWindow = UIObjectManager::CreateObject();
		editWindow->AddComponent<NotesEditUI>(editWindow);
	}
	void NotesEditScene::Final()
	{
#ifdef _DEBUG
		Editor::DebugLog("NotesEditScene::Final");
#endif // _DEBUG
	}
}

