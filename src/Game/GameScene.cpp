#include "pch.h"
#include "GameScene.h"


namespace FlappyBird
{
	void GameScene::Init()
	{
		OutputDebugStringA("GameScene Init\n");
	}
	void GameScene::Final()
	{
		m_gameObjects.clear();
		m_gameObjects.shrink_to_fit();

		for (auto& canvas : m_canvases)
		{
			canvas->Final();
		}
		m_canvases.clear();
		m_canvases.shrink_to_fit();

		OutputDebugStringA("GameScene Final\n");
	}
}
