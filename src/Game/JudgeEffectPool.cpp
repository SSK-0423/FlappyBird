#include "pch.h"
#include "JudgeEffectPool.h"

using namespace Framework;
namespace FlappyBird
{
	JudgeEffectPool::JudgeEffectPool(std::shared_ptr<Object> owner) : IComponent(owner)
	{
		InitJudgeEffectPool();
	}
	JudgeEffectPool::~JudgeEffectPool()
	{
	}
	void JudgeEffectPool::Update(float deltaTime)
	{
	}
	void JudgeEffectPool::Draw()
	{
	}
	void JudgeEffectPool::DrawInspector()
	{
		if (ImGui::CollapsingHeader("JudgeEffectPool"))
		{
			ImGui::Text("PoolSize: %d", m_judgeEffectPool.size());
		}
	}
	JudgeEffect* JudgeEffectPool::GetJudgeEffect()
	{
		// 使用可能なエフェクトを取得する
		for (auto& judgeEffect : m_judgeEffectPool)
		{
			if (!judgeEffect->GetOwner()->GetActive())
			{
				judgeEffect->GetOwner()->SetActive(true);
				return judgeEffect;
			}
		}

		// 使用可能なエフェクトがない場合は新たに生成する
		std::shared_ptr<GameObject> judgeEffectObj = std::shared_ptr<GameObject>(new GameObject());
		m_owner.lock()->AddChild(judgeEffectObj);
		JudgeEffect* judgeEffect = judgeEffectObj->AddComponent<JudgeEffect>(judgeEffectObj);
		m_judgeEffectPool.push_back(judgeEffect);

		return m_judgeEffectPool.back();
	}
	void JudgeEffectPool::InitJudgeEffectPool()
	{
		for (size_t i = 0; i < POOL_SIZE; i++)
		{
			std::shared_ptr<GameObject> judgeEffectObj = std::shared_ptr<GameObject>(new GameObject());
			judgeEffectObj->SetActive(false);
			m_owner.lock()->AddChild(judgeEffectObj);

			JudgeEffect* judgeEffect = judgeEffectObj->AddComponent<JudgeEffect>(judgeEffectObj);
			m_judgeEffectPool.push_back(judgeEffect);
		}
	}
}