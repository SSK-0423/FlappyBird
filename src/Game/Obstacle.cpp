#include "pch.h"
#include "Obstacle.h"

#include "FlappyBird.h"

#include "GameScene.h"
#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace Framework;
using namespace DX12Wrapper;

namespace FlappyBird
{
	// 静的メンバ変数の初期化
	float Obstacle::m_judgeLineX = 0.0f;
	float Obstacle::m_currentPlayTime = 0.0f;
	const int Obstacle::SPACE = 255;

	Obstacle::Obstacle(Framework::Object* owner)
		: IComponent(owner)
	{
		m_owner->SetName("Obstacle");
		m_owner->SetTag("Obstacle");

		auto viewportSize = Dx12GraphicsEngine::GetViewport();

		// 障害物の生成
		m_transforms.resize(2);
		for (size_t i = 0; i < 2; i++)
		{
			GameObject* gameObject = new GameObject();
			gameObject->SetName("Obstacle: " + std::to_string(i));

			Transform2D* transform = gameObject->GetComponent<Transform2D>();
			transform->scale = { 100.f, viewportSize.Height / 1.f };
			if (i == 0)
			{
				transform->position = { viewportSize.Width / 2.f, transform->scale.y / 2.f + SPACE / 2.f };
			}
			else
			{
				transform->angle = 180.f;
				transform->position = { viewportSize.Width / 2.f, transform->scale.y / 2.f - SPACE / 2.f };
			}

			m_transforms[i] = transform;

			RectCollider* collider = gameObject->AddComponent<RectCollider>(gameObject);
			collider->SetRectSize(transform->scale.x, transform->scale.y);

			Rigidbody2D* rigidbody = gameObject->AddComponent<Rigidbody2D>(gameObject);
			rigidbody->useGravity = false;

			Sprite* sprite = new Sprite(L"res/texture/dokan_long.png");
			SpriteRenderer* spriteRenderer = gameObject->AddComponent<SpriteRenderer>(gameObject);
			spriteRenderer->SetSprite(sprite);
			spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GAMEOBJECT);
			spriteRenderer->SetLayer(static_cast<UINT>(SPRITE_LAYER::OBSTACLE));

			gameObject->AddComponent<Material>(gameObject);

			m_owner->AddChild(gameObject);
		}

		m_timing = -1.0f;
	}
	Obstacle::~Obstacle()
	{
	}
	void Obstacle::Update(float deltaTime)
	{
		if (m_timing < 0.0f)
		{
			return;
		}

		// 画面外に出たら非アクティブにする
		if (m_transforms[0]->position.x < -m_transforms[0]->scale.x / 2.f)
		{
			m_owner->SetActive(false);
			m_canPlaySE = true;
		}

		// ノーツのタイミングと判定ラインのタイミングの差を計算
		float diff = m_timing - m_currentPlayTime;

		auto viewportSize = Dx12GraphicsEngine::GetViewport();

		// 画面の右端から判定ラインまでの距離
		float distanceX = viewportSize.Width - m_judgeLineX;

		float x = (diff / 1000.f) * distanceX;

		// 障害物の移動
		for (size_t i = 0; i < m_transforms.size(); i++)
		{
			m_transforms[i]->position.x = m_judgeLineX + x / 2.f;
			if (i == 0)
			{
				m_transforms[i]->position.y = m_posY + m_transforms[i]->scale.y / 2.f + SPACE / 2.f;
			}
			else
			{
				m_transforms[i]->position.y = m_posY - m_transforms[i]->scale.y / 2.f - SPACE / 2.f;
			}
		}
	}
	void Obstacle::Draw()
	{
	}
	void Obstacle::SetMoveSpeed(float moveSpeedX, float moveSpeedY)
	{
		m_moveSpeedX = moveSpeedX;
		m_moveSpeedY = moveSpeedY;
	}
	void Obstacle::SetTiming(float timing)
	{
		m_timing = timing;
	}
	float Obstacle::GetTiming()
	{
		return m_timing;
	}
	void Obstacle::SetPosY(float posY)
	{
		auto viewportSize = Dx12GraphicsEngine::GetViewport();
		m_posY = std::clamp(posY, SPACE * 0.75f, viewportSize.Height - SPACE * 0.75f);
	}
	void Obstacle::SetMaterialColor(const DirectX::XMFLOAT4& color)
	{
		auto children = m_owner->GetChildren();
		for (auto child : children)
		{
			auto material = child->GetComponent<Material>();
			material->SetColor(color);
		}
	}
	bool Obstacle::CanPlaySE()
	{
		return m_canPlaySE;
	}
	void Obstacle::SetCanPlaySE(bool canPlaySE)
	{
		m_canPlaySE = canPlaySE;
	}
	void Obstacle::SetCurrentPlayTime(float currentPlayTime)
	{
		m_currentPlayTime = currentPlayTime;
	}
	void Obstacle::SetJudgeLineX(float judgeLineX)
	{
		m_judgeLineX = judgeLineX;
	}
}
