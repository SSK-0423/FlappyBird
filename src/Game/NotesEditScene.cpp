#include "pch.h"
#include "NotesEditScene.h"
#include "NotesEditUI.h"

#include "../Framework/GameFramework.h"

using namespace Framework;

namespace FlappyBird
{
	void NotesEditScene::Init()
	{
		// TODO: ここに初期化処理を書く
		Editor::DebugLog("NotesEditScene::Init");

		// UI
		UIObject* editWindow = UIObjectManager::CreateObject();
		editWindow->AddComponent<NotesEditUI>(editWindow);
	}
	void NotesEditScene::Final()
	{
		Editor::DebugLog("NotesEditScene::Final");
	}
}

