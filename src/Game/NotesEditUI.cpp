#include "pch.h"
#include "NotesEditUI.h"

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

using namespace Framework;
using namespace Utility;

namespace FlappyBird
{
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
		ImGui::InputText("Music Name", m_fumenData.musicName, 0);

		std::filesystem::path currentPath = std::filesystem::current_path();
		std::filesystem::path soundPath = currentPath / "res" / "sound";

		// "res/sound"�ȉ��̉��y�t�@�C���p�X�̈ꗗ���擾����
		auto musicFilePaths = GetFilePathsInDirectoryWithExtension(soundPath.string(), ".wav");
		static std::string currentItem = "No Select";

		// ���y�t�@�C���p�X�̈ꗗ���h���b�v�_�E���`���ŕ\��
		if (ImGui::BeginCombo("Music File Path", currentItem.c_str()))
		{
			for (int i = 0; i < musicFilePaths.size(); i++)
			{
				bool is_selected = (currentItem == musicFilePaths[i]);
				if (ImGui::Selectable(musicFilePaths[i].c_str(), is_selected))
					currentItem = musicFilePaths[i].c_str();
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		// �I�����ꂽ���y�t�@�C���p�X�𕈖ʃf�[�^�ɐݒ�
		m_fumenData.musicFilePath = currentItem;

		ImGui::InputFloat("BPM", &m_fumenData.bpm, 0);
		ImGui::InputInt("Beat", &m_fumenData.beat, 0);
		ImGui::End();
	}
	void NotesEditUI::DrawSaveLoadButton()
	{
		ImGui::Begin("File");

		// ���ʏ���Json�`���ŕۑ�
		if (ImGui::Button("Save"))
		{
		}
		ImGui::SameLine();

		// ���ʏ���Json�`���œǂݍ���
		if (ImGui::Button("Load"))
		{
		}
		ImGui::End();
	}
}
