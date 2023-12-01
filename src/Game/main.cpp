#include "Framework/Game.h"

#include "Game/FlappyBird.h"
#include "Game/Renderer.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	FlappyBird::FlappyBird flappyBird;
	FlappyBird::Renderer renderer;

	Framework::Game game(flappyBird, renderer);
	// ������
	game.Init();
	// ���s���I���܂Ń��[�v
	game.Run();
	// �I������
	game.Final();

	return 0;
}