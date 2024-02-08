#include "pch.h"
#include "FlappyBird.h"

#include "Framework/SceneManager.h"

#include "TitleScene.h"
#include "MusicSelectScene.h"
#include "GameScene.h"
#include "NotesEditScene.h"

using namespace Framework;

namespace FlappyBird
{
	void FlappyBird::Init()
	{
		SceneManager::RegistScene<TitleScene>("Title");
		SceneManager::RegistScene<GameScene>("Game");
		SceneManager::RegistScene<MusicSelectScene>("MusicSelect");
		SceneManager::RegistScene<NotesEditScene>("NotesEdit");

		SceneManager::SetFirstScene("Title");
	}

	void FlappyBird::Final()
	{
	}
}
