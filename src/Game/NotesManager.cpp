#include "pch.h"
#include "NotesManager.h"

#include "imgui.h"

namespace FlappyBird
{
	NotesManager::NotesManager(Framework::Object* owner) : IComponent(owner)
	{
	}
	NotesManager::~NotesManager()
	{
	}
	void NotesManager::Update(float deltaTime)
	{
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
				ImGui::Separator();
			}
		}
	}
	void NotesManager::CreateNotes(Note data)
	{
		// 既に同じタイミングのノーツが存在する場合は追加しない
		for (auto& note : m_notes)
		{
			if (note.timing == data.timing)
			{
				return;
			}
		}

		m_notes.push_back(data);
	}
	void NotesManager::DeleteNotes(Note data)
	{
		for (auto it = m_notes.begin(); it != m_notes.end(); ++it)
		{
			if (it->timing == data.timing)
			{
				m_notes.erase(it);
				break;
			}
		}
	}
}