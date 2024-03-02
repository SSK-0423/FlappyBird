#pragma once
#include "Framework/GameFramework.h"
#include "JudgeResult.h"

namespace FlappyBird
{
	class JudgeEffectPool;

	class EffectManager : public Framework::IComponent
	{
	public:
		EffectManager(std::shared_ptr<Framework::Object> owner);
		~EffectManager();

		void Start();
		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

	private:
		void SpawnJudgeEffect(JUDGE_RESULT judgeResult);

		JudgeEffectPool* m_judgeEffectPool;
	};
}