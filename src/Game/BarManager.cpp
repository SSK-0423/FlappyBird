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
		// ���݂̍Đ��ʒu���X�V
		UpdateCurrentPlayTime();

		// ���ߐ��̃A�N�e�B�u��Ԃ��X�V
		UpdateBarLineActive();
	}
	void BarManager::Draw()
	{
	}
	void BarManager::DrawInspector()
	{
		if (ImGui::CollapsingHeader("BarManager"))
		{
			// �A�N�e�B�u��Ԃ̏��ߐ�
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
		// �����̏��ߐ����폜
		m_owner.lock()->RemoveAllChildren();
		m_barLines.clear();

		// ���胉�C����x���W���擾
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

				// ����^�C�~���O���v�Z
				// 1���߂̒��� = 1��(60�b) / bpm * ���q
				float barTimeLength = 60.0f * 1000.f / bpm * beat;
				// ���ߓ��̃^�C�~���O���v�Z
				float timing = barTimeLength * i + (barTimeLength / SOUND_NUM_IN_BAR) * j;

				// ���胉�C���̃^�C�~���O��ݒ�
				barLine->SetTiming(timing);

				// ���ߐ��̃^�C�v��ݒ�
				// �S����(���ߓ�)
				if (j == 0)
				{
					barLine->SetBarLineType(BARLINETYPE::WHOLE);
				}
				// 4������
				else if (j % 2 == 0)
				{
					barLine->SetBarLineType(BARLINETYPE::QUARTER);
				}
				// 8������
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
			// �A�N�e�B�u�ł��鏬�ߐ��̃^�C�~���O���擾
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
			// ���ߐ��Ɍ��݂̍Đ��ʒu���Z�b�g
			BarLine::SetCurrentPlayTime(m_musicPlayer->GetCurrentPlayTimeMs());
		}
	}
	void BarManager::UpdateBarLineActive()
	{
		// ���݂̍Đ��ʒu���擾
		float currentPlayTime = m_musicPlayer->GetCurrentPlayTimeMs();

		auto& viewport = Dx12GraphicsEngine::GetViewport();

		// �r���[�|�[�g�̍��[�ƉE�[�̃^�C�~���O���v�Z
		float viewportLeftTime = TimingCalculator::CalcTiming(m_judgeLineX, viewport.TopLeftX, viewport.Width, currentPlayTime);
		float viewportRightTime = TimingCalculator::CalcTiming(m_judgeLineX, viewport.TopLeftX + viewport.Width, viewport.Width, currentPlayTime);

		// ���ߐ����A�N�e�B�u�E��A�N�e�B�u�ɂȂ�u�Ԃ������Ȃ��悤�ɂ��邽�߂ɁA
		// �r���[�|�[�g�̍��[�ƉE�[�̃^�C�~���O�������L����
		viewportLeftTime -= TIMING_OFFSET;
		viewportRightTime += TIMING_OFFSET;

		// ���ߐ��̃A�N�e�B�u��Ԃ��X�V
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