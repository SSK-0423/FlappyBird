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

	NotesEditUI::NotesEditUI(Framework::Object* owner)
		: Framework::IComponent(owner)
	{}
	void NotesEditUI::Update(float deltaTime)
	{
	}
	void NotesEditUI::Draw()
	{
		DrawFumenData();
		DrawSaveLoadButton();
	}
	void NotesEditUI::DrawFumenData()
	{
		ImGui::Begin("Fumen Data");
		ImGui::InputText("Music Name", &m_fumenData.musicName, 0);

		std::filesystem::path soundPath = "res\\sound";
		// "res/sound"�ȉ��̉��y�t�@�C���p�X�̈ꗗ���擾����
		auto musicFilePaths = GetFilePathsInDirectoryWithExtension(soundPath.string(), ".wav");

		//static std::string selectedJsonPath = "No Select";

		// ���y�t�@�C���p�X�̈ꗗ���h���b�v�_�E���`���ŕ\��
		if (ImGui::BeginCombo("Music File Path", m_selectedMusicPath.c_str()))
		{
			for (int i = 0; i < musicFilePaths.size(); i++)
			{
				bool is_selected = (m_selectedMusicPath == musicFilePaths[i]);
				if (ImGui::Selectable(musicFilePaths[i].c_str(), is_selected))
					m_selectedMusicPath = musicFilePaths[i].c_str();
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		// �I�����ꂽ���y�t�@�C���p�X�𕈖ʃf�[�^�ɐݒ�
		m_fumenData.musicFilePath = m_selectedMusicPath;

		ImGui::InputFloat("BPM", &m_fumenData.bpm, 0);
		ImGui::InputInt("Beat", &m_fumenData.beat, 0);
		ImGui::End();
	}
	void NotesEditUI::DrawSaveLoadButton()
	{
		ImGui::Begin("File");

		static std::string selectedJsonPath = "No Select";     // �h���b�v�_�E���őI�����ꂽ�t�@�C����
		static std::string openedFumenPath = "No Opened";       // ���݊J���Ă���t�@�C����
		static std::string actionLog = "";                     // �ۑ���ǂݍ��݂̌��ʂ�\��

		ImGui::Text("Opened Fumen Path: %s", openedFumenPath.c_str());

		// ���ʏ���Json�`���ŕۑ�
		if (ImGui::Button("Save"))
		{
			std::string filePath = "res/fumen/" + m_fumenData.musicName + ".json";
			FumenJsonReadWriter::Write(filePath, m_fumenData);

			openedFumenPath = filePath;
			actionLog = "Saved";
		}
		ImGui::SameLine();

		// ���ʏ���Json�`���œǂݍ���
		if (ImGui::Button("Load"))
		{
			FumenJsonReadWriter::Read(selectedJsonPath, m_fumenData);

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
		if (ImGui::BeginCombo("Fumen File Path", selectedJsonPath.c_str()))
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

		ImGui::End();
	}
}
