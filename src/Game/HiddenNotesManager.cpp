#include "pch.h"
#include "HiddenNotesManager.h"

#include "MusicPlayer.h"

using namespace Framework;

namespace FlappyBird
{
	HiddenNotesManager::HiddenNotesManager(std::shared_ptr<Framework::Object> owner)
		: Framework::IComponent(owner), PERFECT_JUDGE_RANGE(66.66f)
	{
		// SE�ǉ�
		m_seClip = m_owner.lock()->AddComponent<SoundClip>(m_owner.lock());
		m_seClip->LoadWavSound(L"res/sound/clap.wav");
	}

	void HiddenNotesManager::Start()
	{
		// �ȃv���C���[�擾
		m_musicPlayer = GameObjectManager::FindObject("MusicPlayer")->GetComponent<MusicPlayer>();
	}

	void HiddenNotesManager::Update(float deltaTime)
	{
		// �W�����v������B���m�[�c�̔�����s��
		if (InputSystem::GetMouseButtonDown(MOUSECODE::LEFT))
		{
			JudgeTiming();
		}

		// �B���m�[�c�̃A�N�e�B�u��Ԃ��X�V
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
		// �����̉B���m�[�c���폜
		m_hiddenNotes.clear();
		m_hiddenNotes.shrink_to_fit();

		m_hiddenNotes.resize(barNum * 16);

		// �B���m�[�c�𐶐�
		for (unsigned int i = 0; i < barNum; i++)
		{
			for (unsigned int j = 0; j < 16; j++)
			{
				// ����^�C�~���O���v�Z
				// 1���߂̒��� = 1��(60�b) / bpm * ���q
				float barTimeLength = 60.0f * 1000.f / bpm * beat;
				// ���ߓ��̃^�C�~���O���v�Z
				float timing = barTimeLength * i + (barTimeLength / 16.f) * j;

				m_hiddenNotes[i * 16 + j] = HiddenNote(timing, false);
			}
		}
	}
	void HiddenNotesManager::UpdateHiddenNotesState()
	{
		// �B���m�[�c�̃A�N�e�B�u��Ԃ��X�V
		float currentPlayTime = m_musicPlayer->GetCurrentPlayTimeMs();

		for (size_t i = m_currentHiddenNoteIndex; i < m_hiddenNotes.size(); i++)
		{
			auto& hiddenNote = m_hiddenNotes[i];

			if (hiddenNote.isJudged) continue;

			// �Ȃ̌��݂̍Đ��ʒu�ƉB���m�[�c�̃^�C�~���O�̍����v�Z
			float diff = currentPlayTime - hiddenNote.timing;

			// �m�[�c�����胉�C����ʉ߂��������ȉ��̏����Ŕ���
			// ����������͈͂��傫���ꍇ�A�m�[�c�͔��胉�C����ʉ߂��Ă���
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

		// JumpTiming�ɍł��߂��B���m�[�c��T��
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
