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
	void BarManager::Start()
	{
		m_musicPlayer = GameObjectManager::FindObject("MusicPlayer")->GetComponent<MusicPlayer>();
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
		for (auto barLine : m_barLines)
		{
			m_owner->RemoveChild(barLine->GetOwner());
		}
		m_barLines.clear();

		// ���胉�C����x���W���擾
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

				// ����^�C�~���O���v�Z
				// 1���߂̒��� = 1��(60�b) / bpm * ���q
				float barTimeLength = 60.0f * 1000.f / bpm * beat;
				// ���ߓ��̃^�C�~���O���v�Z
				float timing = barTimeLength * i + (barTimeLength / 8.f) * j;

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
		// ���ߐ��̃A�N�e�B�u��Ԃ��X�V
		for (auto barLine : m_barLines)
		{
			float timing = barLine->GetTiming();

			// �m�[�c�̃^�C�~���O�Ɣ��胉�C���̃^�C�~���O�̍����v�Z
			float diff = timing - m_musicPlayer->GetCurrentPlayTimeMs();

			// 1000ms�ȓ��̏ꍇ�A�A�N�e�B�u�ɂ���
			if (0.f < diff && diff < 2000.f)
			{
				barLine->GetOwner()->SetActive(true);
			}
		}
	}
}