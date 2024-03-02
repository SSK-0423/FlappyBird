#pragma once

#include "Framework/GameFramework.h"
#include "JudgeEffect.h"

namespace FlappyBird
{
	/// <summary>
	/// ����G�t�F�N�g�̃I�u�W�F�N�g�v�[��
	/// </summary>
	class JudgeEffectPool : public Framework::IComponent
	{
	public:
		JudgeEffectPool(std::shared_ptr<Framework::Object> owner);
		~JudgeEffectPool();

		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		JudgeEffect* GetJudgeEffect();

	private:
		const size_t POOL_SIZE = 5;
		std::list<JudgeEffect*> m_judgeEffectPool;

		void InitJudgeEffectPool();
	};
}