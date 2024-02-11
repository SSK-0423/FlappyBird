#include "pch.h"
#include "MusicSelectScene.h"
#include "MusicSelectSceneUI.h"
#include "MusicSelect.h"
#include "FlappyBird.h"

#include "Background.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace DX12Wrapper;
using namespace Framework;

namespace FlappyBird
{
	MusicSelectScene::MusicSelectScene()
	{
	}
	void MusicSelectScene::Init()
	{
		std::shared_ptr<UIObject> musicSelectSceneUI = UIObjectManager::CreateObject();
		musicSelectSceneUI->SetName("MusicSelectSceneUI");
		musicSelectSceneUI->AddComponent<MusicSelectSceneUI>(musicSelectSceneUI);

		std::shared_ptr<GameObject> background = GameObjectManager::CreateObject();
		background->SetName("Background");
		background->AddComponent<Background>(background);

		std::shared_ptr<GameObject> musicSelect = GameObjectManager::CreateObject();
		musicSelect->SetName("MusicSelect");
		musicSelect->AddComponent<MusicSelect>(musicSelect);

		// BGM
		std::shared_ptr<GameObject> bgm = GameObjectManager::CreateObject();
		SoundClip* bgmSound = bgm->AddComponent<SoundClip>(bgm);
		bgmSound->LoadWavSound(L"res/sound/music_select_bgm.wav", true);
		bgmSound->Play();

#ifdef _DEBUG
		Editor::DebugLog("MusicSelectScene Init\n");
#endif // _DEBUG
	}
	void MusicSelectScene::Final()
	{
		Scene::Final();

#ifdef _DEBUG
		Editor::DebugLog("MusicSelectScene Final\n");
#endif // _DEBUG
	}
}