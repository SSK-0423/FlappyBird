#include "pch.h"
#include "BarManager.h"

#include "MusicPlayer.h"

#include "imgui.h"

using namespace Framework;

namespace FlappyBird
{
	BarManager::BarManager(Framework::Object* owner) : IComponent(owner)
	{
		SoundClip* soundClip = m_owner->AddComponent<SoundClip>(m_owner);
		soundClip->LoadWavSound(L"res/sound/clap.wav");
	}
	void BarManager::CreateBar(unsigned int barNum, float bpm, int beat)
	{
		m_musicPlayer = GameObjectManager::FindObject("MusicPlayer")->GetComponent<MusicPlayer>();

		// 既存の小節線を削除
		for (auto barLine : m_barLines)
		{
			m_owner->RemoveChild(barLine->GetOwner());
		}
		m_barLines.clear();

		// 判定ラインのx座標を取得
		float judgeLineX = UIObjectManager::FindObject("JudgeLine")->GetComponent<Transform2D>()->position.x;
		BarLine::SetJudgeLineX(judgeLineX);

		for (unsigned int i = 0; i < barNum; i++)
		{
			for (unsigned int j = 0; j < 8; j++)
			{
				UIObject* barLineObj = new UIObject();
				barLineObj->SetName("BarLine: " + std::to_string(i) + "/" + std::to_string(j));
				barLineObj->SetActive(false);

				BarLine* barLine = barLineObj->AddComponent<BarLine>(barLineObj);

				// 判定タイミングを計算
				// 1小節の長さ = 1分(60秒) / bpm * 拍子
				float barTimeLength = 60.0f * 1000.f / bpm * beat;
				// 小節内のタイミングを計算
				float timing = barTimeLength * i + (barTimeLength / 8.f) * j;

				// 判定ラインのタイミングを設定
				barLine->SetTiming(timing);

				m_owner->AddChild(barLineObj);

				m_barLines.push_back(barLine);
			}
		}
	}
	float BarManager::GetNearBarLineTiming(float timing)
	{
		float nearTiming = 0.f;

		float minDiff = 100000.f;

		for (auto barLine : m_barLines)
		{
			// アクティブである小節線のタイミングを取得
			if (barLine->GetOwner()->GetActive())
			{
				float diff = std::abs(barLine->GetTiming() - timing);
				if (diff < minDiff)
				{
					minDiff = diff;
					nearTiming = barLine->GetTiming();
				}
			}
		}

		return nearTiming;
	}
	void BarManager::Update(float deltaTime)
	{
		// 現在の再生位置を更新
		UpdateCurrentPlayTime();

		// 小節線のアクティブ状態を更新
		UpdateBarLineActive();
	}
	void BarManager::Draw()
	{
	}
	void BarManager::DrawInspector()
	{
		if (ImGui::CollapsingHeader("BarManager"))
		{
			// アクティブ状態の小節線
			for (auto barLine : m_barLines)
			{
				if (barLine->GetOwner()->GetActive())
				{
					ImGui::Text("Timing: %f", barLine->GetTiming());
					ImGui::Separator();
				}
			}
		}
	}
	void BarManager::UpdateCurrentPlayTime()
	{
		if (m_musicPlayer != nullptr)
		{
			// 小節線に現在の再生位置をセット
			BarLine::SetCurrentPlayTime(m_musicPlayer->GetCurrentPlayTimeMs());
		}
	}
	void BarManager::UpdateBarLineActive()
	{
		// 小節線のアクティブ状態を更新
		for (auto barLine : m_barLines)
		{
			float timing = barLine->GetTiming();

			// ノーツのタイミングと判定ラインのタイミングの差を計算
			float diff = timing - m_musicPlayer->GetCurrentPlayTimeMs();

			// 1000ms以内の場合、アクティブにする
			if (0.f < diff && diff < 2000.f)
			{
				barLine->GetOwner()->SetActive(true);
			}
			// 判定タイミングを超えたらSEを再生　※デバッグ用
			else if (diff <= 0.f)
			{
				// 以下の条件に合致する場合、SEを再生
				// アクティブである
				// かつ、SEが再生可能である
				if (barLine->GetOwner()->GetActive() &&
					barLine->CanPlaySE())
				{
					m_owner->GetComponent<SoundClip>()->Play();
					barLine->SetCanPlaySE(false);	// 複数回再生できないようにする
				}
			}
		}
	}
}