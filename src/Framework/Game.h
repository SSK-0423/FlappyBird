#pragma once
#include "Window.h"

namespace Framework
{
	class GameImpl;
	class IRenderer;

	static const TCHAR* NAME = L"FlappyBird";

#ifdef _DEBUG
	static const LONG WINDOW_WIDTH = 1920;
	static const LONG WINDOW_HEIGHT = 1080;

	static const LONG GAME_WIDTH = 1024;
	static const LONG GAME_HEIGHT = 768;
#else
	static const LONG WINDOW_WIDTH = 1024;
	static const LONG WINDOW_HEIGHT = 768;
#endif // _DEBUG

	/// <summary>
	/// ゲーム全体を管理するクラス
	/// </summary>
	class Game
	{
	public:
		Game(GameImpl& gameImpl, IRenderer& renderer)
			: m_gameImpl(gameImpl), m_renderer(renderer) {};
		~Game() = default;

	public:

		void Init();

		void Run();

		void Final();

	private:
		std::chrono::system_clock::time_point m_prevFrameTime;
		GameImpl& m_gameImpl;
		IRenderer& m_renderer;
	};
}