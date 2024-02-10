#include "pch.h"
#include "HiddenNotesManager.h"

#include "MusicPlayer.h"

using namespace Framework;

namespace FlappyBird
{
	HiddenNotesManager::HiddenNotesManager(std::shared_ptr<Framework::Object> owner)
		: Framework::IComponent(owner), PERFECT_JUDGE_RANGE(66.66f)
	{
		// SE追加
		m_seClip = m_owner.lock()->AddComponent<SoundClip>(m_owner.lock());
		m_seClip->LoadWavSound(L"res/sound/clap.wav");
	}

	void HiddenNotesManager::Start()
	{
		// 曲プレイヤー取得
		m_musicPlayer = GameObjectManager::FindObject("MusicPlayer")->GetComponent<MusicPlayer>();
	}

	void HiddenNotesManager::Update(float deltaTime)
	{
		// ジャンプしたら隠しノーツの判定を行う
		if (InputSystem::GetMouseButtonDown(MOUSECODE::LEFT))
		{
			JudgeTiming();
		}

		// 隠しノーツのアクティブ状態を更新
		UpdateHiddenNotesState();
	}

	void HiddenNotesManager::Draw()
	{
	}
	void HiddenNotesManager::DrawInspector()
	{
		if (ImGui::CollapsingHeader("HiddenNotesManager"))
		{
			for (auto& hiddenNote : m_hiddenNotes)
			{
				ImGui::Text("Timing: %f", hiddenNote.timing);
				ImGui::Text("IsJudged: %s", hiddenNote.isJudged ? "true" : "false");
			}
		}
	}
	void HiddenNotesManager::CreateHiddenNotes(unsigned int barNum, float bpm, float beat)
	{
		// 既存の隠しノーツを削除
		m_hiddenNotes.clear();
		m_hiddenNotes.shrink_to_fit();

		m_hiddenNotes.resize(barNum * 16);

		// 隠しノーツを生成
		for (unsigned int i = 0; i < barNum; i++)
		{
			for (unsigned int j = 0; j < 16; j++)
			{
				// 判定タイミングを計算
				// 1小節の長さ = 1分(60秒) / bpm * 拍子
				float barTimeLength = 60.0f * 1000.f / bpm * beat;
				// 小節内のタイミングを計算
				float timing = barTimeLength * i + (barTimeLength / 16.f) * j;

				m_hiddenNotes[i * 16 + j] = HiddenNote(timing, false);
			}
		}
	}
	void HiddenNotesManager::UpdateHiddenNotesState()
	{
		// 隠しノーツのアクティブ状態を更新
		float currentPlayTime = m_musicPlayer->GetCurrentPlayTimeMs();

		for (size_t i = m_currentHiddenNoteIndex; i < m_hiddenNotes.size(); i++)
		{
			auto& hiddenNote = m_hiddenNotes[i];

			if (hiddenNote.isJudged) continue;

			// 曲の現在の再生位置と隠しノーツのタイミングの差を計算
			float diff = currentPlayTime - hiddenNote.timing;

			// ノーツが判定ラインを通過したかを以下の条件で判定
			// 差分が判定範囲より大きい場合、ノーツは判定ラインを通過している
			if (diff > PERFECT_JUDGE_RANGE)
			{
				hiddenNote.isJudged = true;
				m_currentHiddenNoteIndex = i;
			}
		}
	}
	void HiddenNotesManager::JudgeTiming()
	{
		float jumpTiming = m_musicPlayer->GetCurrentPlayTimeMs();

		Editor::DebugLog("Jump Timing: %f", jumpTiming);

		// JumpTimingに最も近い隠しノーツを探す
		float minDiff = FLT_MAX;
		HiddenNote& nearestNote = m_hiddenNotes[0];

		for (size_t i = m_currentHiddenNoteIndex; i < m_hiddenNotes.size(); i++)
		{
			float timingDiff = std::abs(m_hiddenNotes[i].timing - jumpTiming);

			if (timingDiff < minDiff)
			{
				minDiff = timingDiff;
				nearestNote = m_hiddenNotes[i];
			}
		}

		Editor::DebugLog("NearestNote Timing: %f", nearestNote.timing);
		Editor::DebugLog("MinDiff: %f", minDiff);

		if (minDiff <= PERFECT_JUDGE_RANGE)
		{
			m_seClip->Play(0.5f);
			nearestNote.isJudged = true;
		}
	}
}
