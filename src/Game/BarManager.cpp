#include "pch.h"
#include "BarManager.h"

#include "MusicPlayer.h"
#include "TimingCalculator.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

#include "imgui.h"

using namespace Framework;
using namespace DX12Wrapper;

namespace FlappyBird
{
	BarManager::BarManager(std::shared_ptr<Object> owner) : IComponent(owner), m_judgeLineX(0.f)
	{
		SoundClip* soundClip = m_owner.lock()->AddComponent<SoundClip>(m_owner.lock());
		soundClip->LoadWavSound(L"res/sound/clap.wav");
	}
	void BarManager::Start()
	{
		m_musicPlayer = GameObjectManager::FindObject("MusicPlayer")->GetComponent<MusicPlayer>();

		m_judgeLineX = UIObjectManager::FindObject("JudgeLine")->GetComponent<Transform2D>()->position.x;
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
	void BarManager::CreateBar(unsigned int barNum, float bpm, int beat)
	{
		// 既存の小節線を削除
		m_owner.lock()->RemoveAllChildren();
		m_barLines.clear();

		// 判定ラインのx座標を取得
		float judgeLineX = UIObjectManager::FindObject("JudgeLine")->GetComponent<Transform2D>()->position.x;
		BarLine::SetJudgeLineX(judgeLineX);

		for (unsigned int i = 0; i < barNum; i++)
		{
			for (unsigned int j = 0; j < SOUND_NUM_IN_BAR; j++)
			{
				std::shared_ptr<UIObject> barLineObj = std::shared_ptr<UIObject>(new UIObject());
				barLineObj->SetName("BarLine: " + std::to_string(i) + "/" + std::to_string(j));
				barLineObj->SetActive(false);

				BarLine* barLine = barLineObj->AddComponent<BarLine>(barLineObj);

				// 判定タイミングを計算
				// 1小節の長さ = 1分(60秒) / bpm * 拍子
				float barTimeLength = 60.0f * 1000.f / bpm * beat;
				// 小節内のタイミングを計算
				float timing = barTimeLength * i + (barTimeLength / SOUND_NUM_IN_BAR) * j;

				// 判定ラインのタイミングを設定
				barLine->SetTiming(timing);

				// 小節線のタイプを設定
				// 全音符(小節頭)
				if (j == 0)
				{
					barLine->SetBarLineType(BARLINETYPE::WHOLE);
				}
				// 4分音符
				else if (j % 2 == 0)
				{
					barLine->SetBarLineType(BARLINETYPE::QUARTER);
				}
				// 8分音符
				else
				{
					barLine->SetBarLineType(BARLINETYPE::EIGHTH);
				}

				m_owner.lock()->AddChild(barLineObj);

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
		// 現在の再生位置を取得
		float currentPlayTime = m_musicPlayer->GetCurrentPlayTimeMs();

		auto& viewport = Dx12GraphicsEngine::GetViewport();

		// ビューポートの左端と右端のタイミングを計算
		float viewportLeftTime = TimingCalculator::CalcTiming(m_judgeLineX, viewport.TopLeftX, viewport.Width, currentPlayTime);
		float viewportRightTime = TimingCalculator::CalcTiming(m_judgeLineX, viewport.TopLeftX + viewport.Width, viewport.Width, currentPlayTime);

		// 小節線がアクティブ・非アクティブになる瞬間を見せないようにするために、
		// ビューポートの左端と右端のタイミングを少し広げる
		viewportLeftTime -= TIMING_OFFSET;
		viewportRightTime += TIMING_OFFSET;

		// 小節線のアクティブ状態を更新
		for (auto barLine : m_barLines)
		{
			float barLineTiming = barLine->GetTiming();

			if (viewportLeftTime <= barLineTiming && barLineTiming <= viewportRightTime)
			{
				barLine->GetOwner()->SetActive(true);
			}
			else
			{
				barLine->GetOwner()->SetActive(false);
			}
		}
	}
}