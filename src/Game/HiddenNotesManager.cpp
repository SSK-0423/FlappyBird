#include "pch.h"
#include "HiddenNotesManager.h"

#include "JumpPoint.h"
#include "MusicPlayer.h"
#include "TimingCalculator.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace DX12Wrapper;
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

		// ���݂̃V�[�����擾
		std::string currentScene = SceneManager::GetCurrentSceneName();
		if (currentScene == "Game")
		{
			// �v���C���[��X���W�𔻒胉�C���Ƃ��Đݒ�
			m_judgeLineX = GameObjectManager::FindObject("Player")->GetComponent<Transform2D>()->position.x;
		}
		else if (currentScene == "NotesEdit")
		{
			// �G�f�B�^�[��X���W�𔻒胉�C���Ƃ��Đݒ�
			m_judgeLineX = UIObjectManager::FindObject("JudgeLine")->GetComponent<Transform2D>()->position.x;
		}
		JumpPoint::SetJudgeLineX(m_judgeLineX);
	}

	void HiddenNotesManager::Update(float deltaTime)
	{
		// GameScene�Ȃ�B���m�[�c�̔�����s��
		if (SceneManager::GetCurrentSceneName() == "Game")
		{
			// �W�����v������B���m�[�c�̔�����s��
			if (InputSystem::GetMouseButtonDown(MOUSECODE::LEFT))
			{
				JudgeTiming();
			}
		}
		else if (SceneManager::GetCurrentSceneName() == "NotesEdit")
		{
			// �G�f�B�^�[�Ȃ�B���m�[�c�̃A�N�e�B�u��Ԃ��X�V
			UpdateHiddenNotesActive();
		}

		// ���݂̍Đ��ʒu���X�V
		UpdateCurrentPlayTime();

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
		//// �����̉B���m�[�c���폜
		//m_hiddenNotes.clear();
		//m_hiddenNotes.shrink_to_fit();

		//m_hiddenNotes.resize(barNum * 16);

		//// �B���m�[�c�𐶐�
		//for (unsigned int i = 0; i < barNum; i++)
		//{
		//	for (unsigned int j = 0; j < 16; j++)
		//	{
		//		// ����^�C�~���O���v�Z
		//		// 1���߂̒��� = 1��(60�b) / bpm * ���q
		//		float barTimeLength = 60.0f * 1000.f / bpm * beat;
		//		// ���ߓ��̃^�C�~���O���v�Z
		//		float timing = barTimeLength * i + (barTimeLength / 16.f) * j;

		//		m_hiddenNotes[i * 16 + j] = HiddenNote(timing, false);
		//	}
		//}
	}
	bool HiddenNotesManager::CreateHiddenNotes(HiddenNoteData data)
	{
		// ���ɓ����^�C�~���O�̉B���m�[�c�����݂���ꍇ�͒ǉ����Ȃ�
		for (auto& hiddenNote : m_hiddenNotes)
		{
			if (hiddenNote.timing == data.timing)
			{
				return false;
			}
		}
		m_hiddenNotes.push_back(data);

		// �B���m�[�c�̐���
		std::shared_ptr<GameObject> hiddenNoteJumpPoint = std::shared_ptr<GameObject>(new GameObject());
		hiddenNoteJumpPoint->SetName("HiddenNoteJumpPoint");
		hiddenNoteJumpPoint->SetActive(false);
		JumpPoint* jumpPoint = hiddenNoteJumpPoint->AddComponent<JumpPoint>(hiddenNoteJumpPoint);
		jumpPoint->SetTiming(data.timing);
		m_jumpPoints.push_back(jumpPoint);

		m_owner.lock()->AddChild(hiddenNoteJumpPoint);

		return true;
	}
	bool HiddenNotesManager::DeleteHiddenNotes(HiddenNoteData data)
	{
		bool couldDeleteData = false;
		bool couldDeleteNote = false;

		// �B���m�[�c�f�[�^���폜
		for (auto it = m_hiddenNotes.begin(); it != m_hiddenNotes.end(); ++it)
		{
			if (it->timing == data.timing)
			{
				m_hiddenNotes.erase(it);
				couldDeleteData = true;
				break;
			}
		}

		// �B���m�[�c���폜
		for (auto it = m_jumpPoints.begin(); it != m_jumpPoints.end(); ++it)
		{
			if ((*it)->GetTiming() == data.timing)
			{
				m_owner.lock()->RemoveChild((*it)->GetOwner());
				m_jumpPoints.erase(it);
				couldDeleteNote = true;
				break;
			}
		}

		return couldDeleteData && couldDeleteNote;
	}
	void HiddenNotesManager::SetHiddenNotes(const std::vector<HiddenNoteData>& hiddenNotes)
	{
		// �����̉B���m�[�c���폜
		m_hiddenNotes.clear();
		m_hiddenNotes.shrink_to_fit();
		m_jumpPoints.clear();
		m_jumpPoints.shrink_to_fit();

		// �^����ꂽ�������ɉB���m�[�c����
		for (auto& hiddenNote : hiddenNotes)
		{
			CreateHiddenNotes(hiddenNote);
		}
	}
	std::vector<HiddenNoteData>& HiddenNotesManager::GetHiddenNotes()
	{
		return m_hiddenNotes;
	}
	void HiddenNotesManager::UpdateHiddenNotesState()
	{
		// �B���m�[�c�̔���󋵂��Ǘ�����
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

		// JumpTiming�ɍł��߂��B���m�[�c��T��
		float minDiff = FLT_MAX;
		HiddenNoteData& nearestNote = m_hiddenNotes[0];

		for (size_t i = m_currentHiddenNoteIndex; i < m_hiddenNotes.size(); i++)
		{
			float timingDiff = std::abs(m_hiddenNotes[i].timing - jumpTiming);

			if (timingDiff < minDiff)
			{
				minDiff = timingDiff;
				nearestNote = m_hiddenNotes[i];
			}
	}

#ifdef _DEBUG
		//Editor::DebugLog("Jump Timing: %f", jumpTiming);
		//Editor::DebugLog("NearestNote Timing: %f", nearestNote.timing);
		//Editor::DebugLog("MinDiff: %f", minDiff);
#endif // _DEBUG

		if (minDiff <= PERFECT_JUDGE_RANGE)
		{
			//m_seClip->Play(0.5f);
			nearestNote.isJudged = true;
		}
}
	void HiddenNotesManager::UpdateCurrentPlayTime()
	{
		if (m_musicPlayer != nullptr)
		{
			// ���݂̍Đ��ʒu���Z�b�g
			JumpPoint::SetCurrentPlayTime(m_musicPlayer->GetCurrentPlayTimeMs());
		}
	}
	void HiddenNotesManager::UpdateHiddenNotesActive()
	{
		// ���݂̍Đ��ʒu���擾
		float currentPlayTime = m_musicPlayer->GetCurrentPlayTimeMs();

		auto& viewport = Dx12GraphicsEngine::GetViewport();

		// �r���[�|�[�g�̍��[�ƉE�[�̃^�C�~���O���v�Z
		float viewportLeftTime = TimingCalculator::CalcTiming(m_judgeLineX, viewport.TopLeftX, viewport.Width, currentPlayTime);
		float viewportRightTime = TimingCalculator::CalcTiming(m_judgeLineX, viewport.TopLeftX + viewport.Width, viewport.Width, currentPlayTime);

		// �m�[�c���A�N�e�B�u�E��A�N�e�B�u�ɂȂ�u�Ԃ������Ȃ��悤�ɂ��邽�߂ɁA
		// �r���[�|�[�g�̍��[�ƉE�[�̃^�C�~���O�������L����
		viewportLeftTime -= TIMING_OFFSET;
		viewportRightTime += TIMING_OFFSET;

		// �m�[�c�̃A�N�e�B�u��Ԃ��X�V
		for (auto note : m_jumpPoints)
		{
			float noteTiming = note->GetTiming();

			if (viewportLeftTime <= noteTiming && noteTiming <= viewportRightTime)
			{
				note->GetOwner()->SetActive(true);
			}
			else
			{
				note->Reset();
				note->GetOwner()->SetActive(false);
			}
		}
	}
}