#include "pch.h"
#include "NotesManager.h"

#include "Obstacle.h"
#include "MusicPlayer.h"

#include "imgui.h"

using namespace Framework;

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
	}
	void NotesManager::Update(float deltaTime)
	{
		// ���݂̍Đ��ʒu���X�V
		UpdateCurrentPlayTime();

		// �m�[�c�̃A�N�e�B�u��Ԃ��X�V
		UpdateNoteActive();
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
		// �m�[�c�̃A�N�e�B�u��Ԃ��X�V
		for (auto note : m_noteObstacles)
		{
			float timing = note->GetTiming();

			// �m�[�c�̃^�C�~���O�Ɣ��胉�C���̃^�C�~���O�̍����v�Z
			float diff = timing - m_musicPlayer->GetCurrentPlayTimeMs();

			// 2000ms�ȓ��̏ꍇ�A�A�N�e�B�u�ɂ���
			if (0.f < diff && diff < 2100.f)
			{
				note->GetOwner()->SetActive(true);
			}
			// ����^�C�~���O�𒴂�����SE���Đ�
			else if (diff <= 0.f)
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