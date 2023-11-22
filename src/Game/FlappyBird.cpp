#include "pch.h"
#include "FlappyBird.h"

#include "Framework/SceneManager.h"
#include "TitleScene.h"

namespace FlappyBird
{
	void FlappyBird::Init()
	{
		auto& sceneManager = Framework::SceneManager::Instance();

		sceneManager.AddScene<TitleScene>("Title");

		sceneManager.LoadScene("Title");
	}

	void FlappyBird::Final()
	{
	}
}
