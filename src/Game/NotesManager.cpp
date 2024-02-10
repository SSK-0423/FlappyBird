#include "pch.h"
#include "NotesManager.h"

#include "Player.h"
#include "Obstacle.h"
#include "MusicPlayer.h"
#include "TimingCalculator.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

#include "imgui.h"

using namespace Framework;
using namespace DX12Wrapper;

namespace FlappyBird
{
	NotesManager::NotesManager(std::shared_ptr<Object> owner) : IComponent(owner)
	{
		SoundClip* soundClip = m_owner.lock()->AddComponent<SoundClip>(m_owner.lock());
		soundClip->LoadWavSound(L"res/sound/clap.wav");
	}
	NotesManager::~NotesManager()
	{
	}
	void NotesManager::Start()
	{
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
		Obstacle::SetJudgeLineX(m_judgeLineX);
	}
	void NotesManager::Update(float deltaTime)
	{
		// ���݂̍Đ��ʒu���X�V
		UpdateCurrentPlayTime();

		// �m�[�c�̃A�N�e�B�u��Ԃ��X�V
		UpdateNoteActive();

		// �m�[�c��SE���Đ�
		PlayNoteSound();
	}
	void NotesManager::Draw()
	{
	}
	void NotesManager::DrawInspector()
	{
		if (ImGui::CollapsingHeader("NotesManager"))
		{
			for (auto& note : m_notes)
			{
				ImGui::Text("Timing: %f", note.timing);
				ImGui::SameLine();
				ImGui::Text("PosY: %f", note.posY);
				ImGui::Separator();
			}
		}
	}
	void NotesManager::CreateNotes(NoteData data)
	{
		// ���ɓ����^�C�~���O�̃m�[�c�����݂���ꍇ�͒ǉ����Ȃ�
		for (auto& note : m_notes)
		{
			if (note.timing == data.timing)
			{
				return;
			}
		}
		m_notes.push_back(data);

		// �m�[�c�̐���
		std::shared_ptr<GameObject> noteObstacle = std::shared_ptr<GameObject>(new GameObject());
		noteObstacle->SetName("NoteObstacle");
		noteObstacle->SetActive(false);
		Obstacle* obstacle = noteObstacle->AddComponent<Obstacle>(noteObstacle);
		obstacle->SetTiming(data.timing);
		obstacle->SetPosY(data.posY);
		m_noteObstacles.push_back(obstacle);

		m_owner.lock()->AddChild(noteObstacle);
	}
	void NotesManager::DeleteNotes(NoteData data)
	{
		for (auto it = m_notes.begin(); it != m_notes.end(); ++it)
		{
			if (it->timing == data.timing)
			{
				m_notes.erase(it);
				break;
			}
		}

		for (auto it = m_noteObstacles.begin(); it != m_noteObstacles.end(); ++it)
		{
			if ((*it)->GetTiming() == data.timing)
			{
				m_owner.lock()->RemoveChild((*it)->GetOwner());
				m_noteObstacles.erase(it);
				break;
			}
		}
	}
	std::vector<NoteData>& NotesManager::GetNotes()
	{
		return m_notes;
	}
	void NotesManager::SetNotes(const std::vector<NoteData>& notes)
	{
		// �����̃m�[�c���폜
		m_notes.clear();
		m_notes.shrink_to_fit();
		m_owner.lock()->RemoveAllChildren();
		m_noteObstacles.clear();
		m_noteObstacles.shrink_to_fit();

		for (auto& note : notes)
		{
			CreateNotes(note);
		}

		Editor::DebugLog("Notes Set Completed");
	}
	void NotesManager::UpdateCurrentPlayTime()
	{
		if (m_musicPlayer != nullptr)
		{
			// ���ߐ��Ɍ��݂̍Đ��ʒu���Z�b�g
			Obstacle::SetCurrentPlayTime(m_musicPlayer->GetCurrentPlayTimeMs());
		}
	}
	void NotesManager::UpdateNoteActive()
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
		for (auto note : m_noteObstacles)
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
	void NotesManager::PlayNoteSound()
	{
		float currentPlayTime = m_musicPlayer->GetCurrentPlayTimeMs();

		for (auto note : m_noteObstacles)
		{
			// �m�[�c�̃^�C�~���O�Ɣ��胉�C���̃^�C�~���O�̍����v�Z
			float diff = note->GetTiming() - currentPlayTime;

			// ����^�C�~���O�𒴂�����SE���Đ�
			if (std::abs(diff) <= 16.f)
			{
				// �ȉ��̏����ɍ��v����ꍇ�ASE���Đ�
				// �A�N�e�B�u�ł���
				// ���ASE���Đ��\�ł���
				if (note->GetOwner()->GetActive() &&
					note->CanPlaySE())
				{
					m_owner.lock()->GetComponent<SoundClip>()->Play(0.5f);
					note->SetCanPlaySE(false);	// ������Đ��ł��Ȃ��悤�ɂ���
				}
			}
		}
	}
}