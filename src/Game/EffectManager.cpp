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
		// �B���m�[�c���莞�ɃG�t�F�N�g��\��
		auto hiddenNotesManager = GameObjectManager::FindObject("HiddenNotesManager")->GetComponent<HiddenNotesManager>();
		hiddenNotesManager->OnJudgeTiming.Subscribe([this](JUDGE_RESULT judgeResult) {SpawnJudgeEffect(judgeResult); });

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
		// �G�t�F�N�g�v�[������G�t�F�N�g���擾
		auto judgeEffect = m_judgeEffectPool->GetJudgeEffect();
		Editor::DebugLog("SpawnJudgeEffect");
	}
}