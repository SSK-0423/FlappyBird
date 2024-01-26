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
#ifdef _DEBUG
		Editor::DebugLog("NotesEditScene::Init");
#endif // _DEBUG

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

