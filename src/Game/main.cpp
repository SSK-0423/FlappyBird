#include "Framework/Game.h"

#include "Game/FlappyBird.h"
#include "Game/Renderer.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	FlappyBird::FlappyBird flappyBird;
	FlappyBird::Renderer renderer;

	Framework::Game game(flappyBird, renderer);
	// 初期化
	game.Init();
	// 実行が終わるまでループ
	game.Run();
	// 終了処理
	game.Final();

	return 0;
}