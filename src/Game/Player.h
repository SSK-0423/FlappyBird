#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class GameMaster;

	/// <summary>
	/// �v���C���[�̋������Ǘ�����R���|�[�l���g
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
		/// �v���C���[����Q���ɓ����������ǂ���
		/// </summary>
		/// <returns>true:�Փˁ@false�F���Փ�</returns>
		bool IsDead();
	private:
		GameMaster* m_gameMaster;

		/// <summary>
		/// �v���C���[�̏��
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

		bool m_isAlive;	// �����Ă���(�܂���Q���ɓ������Ă��Ȃ�)���ǂ���

		/// <summary>
		/// �v���C���[����Q���ɓ������Ď��S�����Ƃ��ɌĂ΂��
		/// </summary>
		void OnDead();
	};
}