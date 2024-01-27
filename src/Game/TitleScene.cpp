#include "pch.h"
#include "TitleScene.h"

#include "Framework/Window.h"

#include "Framework/UIObjectManager.h"

#include "Framework/Sprite.h"
#include "Framework/SpriteRenderer.h"
#include "Framework/Text.h"
#include "Framework/Transform2D.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

#include "PleaseClickText.h"
#include "TitleSceneUI.h"

using namespace Framework;
using namespace DX12Wrapper;

namespace FlappyBird
{
	void TitleScene::Init()
	{
		auto viewportSize = Dx12GraphicsEngine::GetViewport();

		// ”wŒi
		GameObject* background = GameObjectManager::CreateObject();
		background->SetName("Background");
		Sprite* backgroundSprite = new Sprite(L"res/texture/sky.png");
		SpriteRenderer* backgroundRenderer = background->AddComponent<SpriteRenderer>(background);
		backgroundRenderer->SetSprite(backgroundSprite);
		backgroundRenderer->SetDrawMode(SPRITE_DRAW_MODE::GUI);
		backgroundRenderer->SetLayer(static_cast<UINT>(TITLE_SCENE_LAYER::BACKGROUND));
		Transform2D* backgroundTransform = background->GetComponent<Transform2D>();
		backgroundTransform->position = { viewportSize.Width / 2.f, viewportSize.Height / 2.f };
		backgroundTransform->scale = { viewportSize.Width * 1.f, viewportSize.Height * 1.f };

		// BGM
		GameObject* bgm = GameObjectManager::CreateObject();
		SoundClip* bgmSound = bgm->AddComponent<SoundClip>(bgm);
		bgmSound->LoadWavSound(L"res/sound/title_bgm.wav", true);
		bgmSound->Play();

		// UI
		UIObject* titleSceneUI = UIObjectManager::CreateObject();
		titleSceneUI->SetName("TitleSceneUI");
		titleSceneUI->AddComponent<TitleSceneUI>(titleSceneUI);

#ifdef _DEBUG
		Editor::DebugLog("TitleScene Init\n");
#endif // _DEBUG
	}
	void TitleScene::Final()
	{
		Scene::Final();
#ifdef _DEBUG
		Editor::DebugLog("TitleScene Final\n");
#endif // _DEBUG
	}
}