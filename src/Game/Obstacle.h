#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	/// <summary>
	/// 障害物(ノーツ)コンポーネント
	/// </summary>
	class Obstacle : public Framework::IComponent
	{
	public:
		Obstacle(std::shared_ptr<Framework::Object> owner);
		~Obstacle();

		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		void SetTiming(float timing);
		float GetTiming();

		void SetPosY(float posY);
		void SetMaterialColor(const DirectX::XMFLOAT4& color);

		bool CanPlaySE();
		void SetCanPlaySE(bool canPlaySE);
		
		void Reset();

		static void SetJudgeLineX(float judgeLineX);
		static void SetCurrentPlayTime(float currentPlayTime);

	private:
		float m_timing = 0.0f;
		float m_posY = 0.0f;

		bool m_canPlaySE = true;

		Framework::Transform2D* m_overObstacleTransform = nullptr;
		Framework::Transform2D* m_underObstacleTransform = nullptr;

		void UpdatePosition();

		static const int SPACE;
		static float m_judgeLineX;
		static float m_currentPlayTime;
		static UINT m_obstacleCount;
	};
}