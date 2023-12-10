#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	enum class GAME_STATE
	{
		READY,
		PLAYING,
		GAMEOVER,
	};

	/// <summary>
	/// �Q�[���S�̂̐i�s���Ǘ�����N���X
	/// </summary>
	class GameMaster : public Framework::IComponent
	{
	public:
		GameMaster(Framework::Object* owner);
		~GameMaster() = default;

		void Update(float deltaTime) override;
		void Draw() override;

		void ChangeState(GAME_STATE state);
		GAME_STATE GetGameState();
	private:
		GAME_STATE m_gameState;

		float m_elapsedTime;
		float m_gameStartTime;

		void GameReady(float deltaTime);
		void GameOver(float deltaTime);
	};
}