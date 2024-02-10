#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	enum class GAME_STATE
	{
		READY,
		PLAYING,
		GAMECLEAR,
		GAMEOVER,
	};

	/// <summary>
	/// ゲーム全体の進行を管理するクラス
	/// </summary>
	class GameMaster : public Framework::IComponent
	{
	public:
		GameMaster(std::shared_ptr<Framework::Object> owner);
		~GameMaster() = default;

		void Start() override;
		void Update(float deltaTime) override;
		void Draw() override;

		void ChangeState(GAME_STATE state);
		GAME_STATE GetGameState();
	private:
		GAME_STATE m_gameState;
		std::shared_ptr<Framework::UIObject> m_gameReadyUI;
		std::shared_ptr<Framework::UIObject> m_gameClearUI;
		std::shared_ptr<Framework::UIObject> m_gameOverUI;

		float m_elapsedTime;
		float m_gameStartTime;

		void GameReady(float deltaTime);
		void GameOver(float deltaTime);

		void OnGameReady();
		void OnGameStart();
		void OnGameClear();
		void OnGameOver();
	};
}