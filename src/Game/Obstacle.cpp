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
	UINT Obstacle::m_obstacleCount = 0;

	Obstacle::Obstacle(std::shared_ptr<Framework::Object> owner)
		: IComponent(owner)
	{
		m_owner.lock()->SetName("Obstacle " + std::to_string(m_obstacleCount));
		m_owner.lock()->SetTag("Obstacle");
		m_obstacleCount++;

		auto& viewportSize = Dx12GraphicsEngine::GetViewport();

		// 上側の障害物生成
		std::shared_ptr<GameObject> underObstacle = std::shared_ptr<GameObject>(new GameObject());
		underObstacle->SetName("UnderObstacle");
		underObstacle->SetTag("Obstacle");
		m_underObstacleTransform = underObstacle->GetComponent<Transform2D>();
		m_underObstacleTransform->scale = { 100.f, viewportSize.Height / 1.f };
		m_underObstacleTransform->position = { viewportSize.Width / 2.f, m_underObstacleTransform->scale.y / 2.f + SPACE / 2.f };

		RectCollider* collider = underObstacle->AddComponent<RectCollider>(underObstacle);
		collider->SetRectSize(m_underObstacleTransform->scale.x, m_underObstacleTransform->scale.y);

		Sprite* sprite = new Sprite(L"res/texture/dokan_long.png");
		SpriteRenderer* spriteRenderer = underObstacle->AddComponent<SpriteRenderer>(underObstacle);
		spriteRenderer->SetSprite(sprite);
		spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GAMEOBJECT);
		spriteRenderer->SetLayer(static_cast<UINT>(SPRITE_LAYER::OBSTACLE));

		underObstacle->AddComponent<Material>(underObstacle);

		m_owner.lock()->AddChild(underObstacle);

		// 下側の障害物生成
		std::shared_ptr<GameObject> overObstacle = std::shared_ptr<GameObject>(new GameObject());
		overObstacle->SetName("OverObstacle");
		overObstacle->SetTag("Obstacle");
		m_overObstacleTransform = overObstacle->GetComponent<Transform2D>();
		m_overObstacleTransform->scale = { 100.f, viewportSize.Height / 1.f };
		m_overObstacleTransform->position = { viewportSize.Width / 2.f, m_overObstacleTransform->scale.y / 2.f - SPACE / 2.f };
		m_overObstacleTransform->angle = 180.f;

		collider = overObstacle->AddComponent<RectCollider>(overObstacle);
		collider->SetRectSize(m_overObstacleTransform->scale.x, m_overObstacleTransform->scale.y);

		sprite = new Sprite(L"res/texture/dokan_long.png");
		spriteRenderer = overObstacle->AddComponent<SpriteRenderer>(overObstacle);
		spriteRenderer->SetSprite(sprite);
		spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GAMEOBJECT);
		spriteRenderer->SetLayer(static_cast<UINT>(SPRITE_LAYER::OBSTACLE));

		overObstacle->AddComponent<Material>(overObstacle);

		m_owner.lock()->AddChild(overObstacle);

		m_owner.lock()->SetActive(false);

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

		UpdatePosition();
	}
	void Obstacle::Draw()
	{
	}
	void Obstacle::DrawInspector()
	{
		if (ImGui::CollapsingHeader("Obstacle"))
		{
			ImGui::Text("Timing: %f", m_timing);
		}
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
		auto children = m_owner.lock()->GetChildren();
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
	void Obstacle::UpdatePosition()
	{
		// ノーツのタイミングと判定ラインのタイミングの差を計算
		float diff = m_timing - m_currentPlayTime;

		auto& viewportSize = Dx12GraphicsEngine::GetViewport();

		// 画面の右端から判定ラインまでの距離
		float distanceX = viewportSize.Width - m_judgeLineX;

		float x = (diff / 1000.f) * distanceX;

		// 障害物の移動
		float obstacleNewPosX = m_judgeLineX + x / 2.f;
		float halfObstacleScaleY = m_overObstacleTransform->scale.y / 2.f;
		float halfSpace = SPACE / 2.f;

		m_underObstacleTransform->position.x = obstacleNewPosX;
		m_underObstacleTransform->position.y = m_posY + halfObstacleScaleY + halfSpace;

		m_overObstacleTransform->position.x = obstacleNewPosX;
		m_overObstacleTransform->position.y = m_posY - halfObstacleScaleY - halfSpace;
	}
	void Obstacle::Reset()
	{
		m_owner.lock()->SetActive(false);
		m_canPlaySE = true;
	}
	void Obstacle::SetJudgeLineX(float judgeLineX)
	{
		m_judgeLineX = judgeLineX;
	}
}
