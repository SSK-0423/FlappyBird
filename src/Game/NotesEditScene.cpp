#include "pch.h"
#include "NotesEditScene.h"
#include "NotesEditUI.h"

#include "../Framework/GameFramework.h"

using namespace Framework;

namespace FlappyBird
{
	void NotesEditScene::Init()
	{
		// TODO: ‚±‚±‚É‰Šú‰»ˆ—‚ð‘‚­
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

