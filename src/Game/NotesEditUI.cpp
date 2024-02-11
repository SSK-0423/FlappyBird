#include "pch.h"
#include "NotesEditUI.h"
#include "FumenJsonReadWriter.h"

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
#include "imgui_stdlib.h"

using namespace Framework;
using namespace Utility;

namespace FlappyBird
{
	// �ÓI�����o�ϐ��̏�����
	std::string NotesEditUI::m_selectedMusicPath = "No Select";
	const ImVec2 NotesEditUI::WINDOW_POS = ImVec2(0, 0);
	const ImVec2 NotesEditUI::WINDOW_SIZE = ImVec2(300, 600);

	NotesEditUI::NotesEditUI(std::shared_ptr<Object> owner)
		: Framework::IComponent(owner)
	{}
	void NotesEditUI::Update(float deltaTime)
	{
	}
	void NotesEditUI::Draw()
	{
		ImGui::SetNextWindowPos(WINDOW_POS);
		ImGui::SetNextWindowSize(WINDOW_SIZE);
		ImGui::Begin("EditWindow");

		ImGui::SeparatorText("Save/Load");
		DrawSaveLoadButton();

		ImGui::SeparatorText("Music Play/Stop");
		DrawPlayStopRestartButton();

		ImGui::SeparatorText("Edit Start");
		DrawEditStartButton();

		ImGui::SeparatorText("Fumen Data");
		DrawFumenData();

		ImGui::End();
	}
	void NotesEditUI::DrawFumenData()
	{
		ImGui::InputText("Music Name", &m_fumenData.musicName, 0);

		std::filesystem::path soundPath = "res/sound";
		// "res/sound"�ȉ��̉��y�t�@�C���p�X�̈ꗗ���擾����
		auto musicFilePaths = GetFilePathsInDirectoryWithExtension(soundPath.string(), ".wav");

		// ���y�t�@�C���p�X�̈ꗗ���h���b�v�_�E���`���ŕ\��
		if (ImGui::BeginCombo("Music File", m_selectedMusicPath.c_str()))
		{
			for (int i = 0; i < musicFilePaths.size(); i++)
			{
				bool is_selected = (m_selectedMusicPath == musicFilePaths[i]);

				if (ImGui::Selectable(musicFilePaths[i].c_str(), is_selected))
				{
					m_selectedMusicPath = musicFilePaths[i].c_str();
				}

				// �I�����ꂽ���y�t�@�C���p�X�Ƀt�H�[�J�X�𓖂Ă�
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		// �I�����ꂽ���y�t�@�C���p�X�𕈖ʃf�[�^�ɐݒ�
		m_fumenData.musicFilePath = m_selectedMusicPath;

		ImGui::InputFloat("BPM", &m_fumenData.bpm, 0);
		ImGui::InputInt("Beat", &m_fumenData.beat, 0);
	}
	void NotesEditUI::DrawSaveLoadButton()
	{
		static std::string selectedJsonPath = "No Select";     // �h���b�v�_�E���őI�����ꂽ�t�@�C����
		static std::string openedFumenPath = "No Opened";       // ���݊J���Ă���t�@�C����
		static std::string actionLog = "";                     // �ۑ���ǂݍ��݂̌��ʂ�\��

		ImGui::Text("Opened Fumen Path: %s", openedFumenPath.c_str());

		// ���ʏ���Json�`���ŕۑ�
		if (ImGui::Button("Save"))
		{
			std::string savePath = "res\\fumen\\" + m_fumenData.musicName + ".json";
			OnSave.Notify(savePath, m_fumenData);

			// �ۑ��������ʃt�@�C���̃p�X��\��
			openedFumenPath = savePath;
			actionLog = "Saved";
		}
		ImGui::SameLine();

		// ���ʏ���Json�`���œǂݍ���
		if (ImGui::Button("Load"))
		{
			OnLoad.Notify(selectedJsonPath, m_fumenData);

			// ���y�t�@�C���p�X��ǂݍ��܂ꂽ���̂ɐݒ�
			m_selectedMusicPath = m_fumenData.musicFilePath;
			openedFumenPath = selectedJsonPath;
			actionLog = "Loaded";
		}

		// ���ʃt�@�C���̃p�X�ꗗ���h���b�v�_�E���`���ŕ\��
		std::filesystem::path fumenPath = "res\\fumen";

		// "res/sound"�ȉ��̉��y�t�@�C���p�X�̈ꗗ���擾����
		auto fumenFilePaths = GetFilePathsInDirectoryWithExtension(fumenPath.string(), ".json");

		// ���ʃt�@�C���̈ꗗ���h���b�v�_�E���`���ŕ\��
		if (ImGui::BeginCombo("Fumen File", selectedJsonPath.c_str()))
		{
			for (int i = 0; i < fumenFilePaths.size(); i++)
			{
				bool is_selected = (selectedJsonPath == fumenFilePaths[i]);
				if (ImGui::Selectable(fumenFilePaths[i].c_str(), is_selected))
					selectedJsonPath = fumenFilePaths[i].c_str();
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::Text(actionLog.c_str());
	}
	void NotesEditUI::DrawPlayStopRestartButton()
	{
		if (ImGui::Button("Play"))
		{
			// �Ȃ��Đ�
			OnPlay.Notify(NotificationEvent());
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop"))
		{
			// �Ȃ��~
			OnStop.Notify(NotificationEvent());
		}
		ImGui::SameLine();
		if (ImGui::Button("Restart"))
		{
			// �Ȃ��ŏ�����Đ�
			OnRestart.Notify(NotificationEvent());
		}
	}
	void NotesEditUI::DrawEditStartButton()
	{
		if (ImGui::Button("Edit Start"))
		{
			// �ҏW�J�n
			OnEditStart.Notify(m_fumenData);
		}
	}
}
