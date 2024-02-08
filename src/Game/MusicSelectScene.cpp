#include "pch.h"
#include "MusicSelectScene.h"

using namespace Framework;

namespace FlappyBird
{
	MusicSelectScene::MusicSelectScene()
	{
	}
	void MusicSelectScene::Init()
	{
#ifdef _DEBUG
		Editor::DebugLog("MusicSelectScene Init\n");
#endif // _DEBUG
	}
	void MusicSelectScene::Final()
	{
#ifdef _DEBUG
		Editor::DebugLog("MusicSelectScene Final\n");
#endif // _DEBUG
	}
}