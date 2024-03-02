#include "pch.h"
#include "EffectManager.h"

#include "JudgeEffectPool.h"
#include "HiddenNotesManager.h"

using namespace Framework;
namespace FlappyBird
{
	EffectManager::EffectManager(std::shared_ptr<Framework::Object> owner) : IComponent(owner)
	{
	}
	EffectManager::~EffectManager()
	{
	}
	void EffectManager::Start()
	{
		// 隠しノーツ判定時にエフェクトを表示
		auto hiddenNotesManager = GameObjectManager::FindObject("HiddenNotesManager")->GetComponent<HiddenNotesManager>();
		hiddenNotesManager->OnJudge.Subscribe([this](JUDGE_RESULT judgeResult) {SpawnJudgeEffect(judgeResult); });

		m_judgeEffectPool = GameObjectManager::FindObject("JudgeEffectPool")->GetComponent<JudgeEffectPool>();
	}
	void EffectManager::Update(float deltaTime)
	{
	}
	void EffectManager::Draw()
	{
	}
	void EffectManager::DrawInspector()
	{
	}
	void EffectManager::SpawnJudgeEffect(JUDGE_RESULT judgeResult)
	{
		// エフェクトプールからエフェクトを取得
		auto judgeEffect = m_judgeEffectPool->GetJudgeEffect();

		switch (judgeResult)
		{
		case JUDGE_RESULT::PERFECT:
			judgeEffect->SetJudgeText(JudgeText::PERFECT_TEXT);
			judgeEffect->SetJudgeTextColor(JudgeColor::PERFECT_COLOR);
			break;
		case JUDGE_RESULT::GREAT:
			judgeEffect->SetJudgeText(JudgeText::GREAT_TEXT);
			judgeEffect->SetJudgeTextColor(JudgeColor::GREAT_COLOR);
			break;
		case JUDGE_RESULT::GOOD:
			judgeEffect->SetJudgeText(JudgeText::GOOD_TEXT);
			judgeEffect->SetJudgeTextColor(JudgeColor::GOOD_COLOR);
			break;
		default:
			break;
		}

		Editor::DebugLog("SpawnJudgeEffect");
	}
}