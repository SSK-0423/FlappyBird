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
		editWindow->SetName("NotesEditUI");
		editWindow->AddComponent<NotesEditUI>(editWindow);

		// ���ߐ��̊Ǘ��I�u�W�F�N�g
		UIObject* barManagerObj = UIObjectManager::CreateObject();
		barManagerObj->SetName("BarManager");
		BarManager* barManager = barManagerObj->AddComponent<BarManager>(barManagerObj);

		//// ���[���̊Ǘ��I�u�W�F�N�g
		//UIObject* laneManagerObj = UIObjectManager::CreateObject();
		//laneManagerObj->SetName("LaneManager");

		// �m�[�c�̊Ǘ��I�u�W�F�N�g
		GameObject* notesManagerObj = GameObjectManager::CreateObject();
		notesManagerObj->SetName("NotesManager");
		NotesManager* notesManager = notesManagerObj->AddComponent<NotesManager>(notesManagerObj);

		// �ȍĐ��Ǘ��I�u�W�F�N�g
		GameObject* musicPlayerObj = GameObjectManager::CreateObject();
		musicPlayerObj->SetName("MusicPlayer");
		MusicPlayer* musicPlayer = musicPlayerObj->AddComponent<MusicPlayer>(musicPlayerObj);

		// �m�[�c�G�f�B�^�[�̊Ǘ��I�u�W�F�N�g
		GameObject* notesEditorObj = GameObjectManager::CreateObject();
		notesEditorObj->SetName("NotesEditor");
		NotesEditor* notesEditor = notesEditorObj->AddComponent<NotesEditor>(notesEditorObj);
		notesEditor->Init();
	}

	void NotesEditScene::Final()
	{
#ifdef _DEBUG
		Editor::DebugLog("NotesEditScene::Final");
#endif // _DEBUG
	}
}

