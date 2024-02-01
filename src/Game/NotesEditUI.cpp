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
	// 静的メンバ変数の初期化
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
		// "res/sound"以下の音楽ファイルパスの一覧を取得する
		auto musicFilePaths = GetFilePathsInDirectoryWithExtension(soundPath.string(), ".wav");

		//static std::string selectedJsonPath = "No Select";

		// 音楽ファイルパスの一覧をドロップダウン形式で表示
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

		// 選択された音楽ファイルパスを譜面データに設定
		m_fumenData.musicFilePath = m_selectedMusicPath;

		ImGui::InputFloat("BPM", &m_fumenData.bpm, 0);
		ImGui::InputInt("Beat", &m_fumenData.beat, 0);
		ImGui::End();
	}
	void NotesEditUI::DrawSaveLoadButton()
	{
		ImGui::Begin("File");

		static std::string selectedJsonPath = "No Select";     // ドロップダウンで選択されたファイル名
		static std::string openedFumenPath = "No Opened";       // 現在開いているファイル名
		static std::string actionLog = "";                     // 保存や読み込みの結果を表示

		ImGui::Text("Opened Fumen Path: %s", openedFumenPath.c_str());

		// 譜面情報をJson形式で保存
		if (ImGui::Button("Save"))
		{
			std::string filePath = "res/fumen/" + m_fumenData.musicName + ".json";
			FumenJsonReadWriter::Write(filePath, m_fumenData);

			openedFumenPath = filePath;
			actionLog = "Saved";
		}
		ImGui::SameLine();

		// 譜面情報をJson形式で読み込み
		if (ImGui::Button("Load"))
		{
			FumenJsonReadWriter::Read(selectedJsonPath, m_fumenData);

			// 音楽ファイルパスを読み込まれたものに設定
			m_selectedMusicPath = m_fumenData.musicFilePath;
			openedFumenPath = selectedJsonPath;
			actionLog = "Loaded";
		}

		// 譜面ファイルのパス一覧をドロップダウン形式で表示
		std::filesystem::path fumenPath = "res\\fumen";

		// "res/sound"以下の音楽ファイルパスの一覧を取得する
		auto fumenFilePaths = GetFilePathsInDirectoryWithExtension(fumenPath.string(), ".json");

		// 譜面ファイルの一覧をドロップダウン形式で表示
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
