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
#include "Background.h"

using namespace Framework;
using namespace DX12Wrapper;

namespace FlappyBird
{
	void TitleScene::Init()
	{
		auto viewportSize = Dx12GraphicsEngine::GetViewport();

		// îwåi
		std::shared_ptr<GameObject> background = GameObjectManager::CreateObject();
		background->AddComponent<Background>(background);

		// BGM
		std::shared_ptr<GameObject> bgm = GameObjectManager::CreateObject();
		SoundClip* bgmSound = bgm->AddComponent<SoundClip>(bgm);
		bgmSound->LoadWavSound(L"res/sound/title_bgm.wav", true);
		bgmSound->Play();

		// UI
		std::shared_ptr<UIObject> titleSceneUI = UIObjectManager::CreateObject();
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