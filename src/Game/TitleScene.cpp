#include "pch.h"
#include "TitleScene.h"
#include "TitleSceneCanvas.h"

#include "Framework/Sprite.h"

using namespace Framework;

namespace FlappyBird
{
	void TitleScene::Init()
	{
		// CanvasçÏê¨
		std::unique_ptr<TitleSceneCanvas> titleCanvas = std::make_unique<TitleSceneCanvas>();
		titleCanvas->Init();
		m_canvases.push_back(std::move(titleCanvas));

		OutputDebugStringA("TitleScene Init\n");
	}
	void TitleScene::Final()
	{
		m_gameObjects.clear();
		m_gameObjects.shrink_to_fit();

		for (auto& canvas : m_canvases)
		{
			canvas->Final();
		}
		m_canvases.clear();
		m_canvases.shrink_to_fit();

		OutputDebugStringA("TitleScene Final\n");
	}
}