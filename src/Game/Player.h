#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class GameMaster;

	/// <summary>
	/// プレイヤーの挙動を管理するコンポーネント
	/// </summary>
	class Player : public Framework::IComponent
	{
	public:
		Player(std::shared_ptr<Framework::Object> owner);
		~Player() = default;

		void Start() override;
		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		void OnCollision(Framework::Collider* other);

		/// <summary>
		/// プレイヤーが障害物に当たったかどうか
		/// </summary>
		/// <returns>true:衝突　false：未衝突</returns>
		bool IsDead();
	private:
		GameMaster* m_gameMaster;

		/// <summary>
		/// プレイヤーの状態
		/// </summary>
		enum class STATE
		{
			FALL,
			JUMP,
			DEAD
		};

		std::unordered_map<STATE, std::shared_ptr<Framework::Sprite>> m_stateSprites;

		std::shared_ptr<Framework::Sprite> m_jumpSprite;
		std::shared_ptr<Framework::Sprite> m_fallSprite;
		std::shared_ptr<Framework::Sprite> m_deadSprite;
		void ChangeSprite();

		Framework::SpriteRenderer* m_spriteRenderer;
		Framework::Transform2D* m_transform;
		Framework::Rigidbody2D* m_rigidbody;
		Framework::SoundClip* m_jumpSound;
		Framework::SoundClip* m_damageSound;
		Framework::SoundClip* m_fallSound;

		float m_jumpVelocity;
		void Move(float deltaTime);
		void Jump();
		void LimitPosition();

		float m_elapsedTime;
		float m_gameReadyAnimationInterval;
		void GameReadyAnimation(float deltaTime);
		void GameOverAnimation(float deltaTime);

		bool m_isAlive;	// 生きている(まだ障害物に当たっていない)かどうか

		/// <summary>
		/// プレイヤーが障害物に当たって死亡したときに呼ばれる
		/// </summary>
		void OnDead();
	};
}