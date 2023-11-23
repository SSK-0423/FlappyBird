#pragma once
#include "Window.h"

namespace Framework
{
	class GameImpl;
	class IRenderer;

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