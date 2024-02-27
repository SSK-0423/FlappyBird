#include "pch.h"
#include "NotesEditUI.h"
#include "FumenJsonReadWriter.h"

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
#include "imgui_stdlib.h"

#include "FlappyBird.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace DX12Wrapper;
using namespace Framework;
using namespace Utility;

namespace FlappyBird
{
	// 静的メンバ変数の初期化
	std::string NotesEditUI::m_selectedMusicPath = "No Select";
	const ImVec2 NotesEditUI::WINDOW_POS = ImVec2(0, 0);
	const ImVec2 NotesEditUI::WINDOW_SIZE = ImVec2(300, 405);
	FumenData NotesEditUI::m_fumenData;
	int NotesEditUI::m_selectedNoteType = static_cast<int>(NoteType::OBSTACLE);

	NotesEditUI::NotesEditUI(std::shared_ptr<Object> owner)
		: Framework::IComponent(owner)
	{
		// 操作方法を示すUI
		auto& viewport = Dx12GraphicsEngine::GetViewport();
		std::shared_ptr<Framework::UIObject> operationText = UIObjectManager::CreateObject();
		Text* text = operationText->AddComponent<Text>(operationText);
		text->SetText(L"Mouse:Put/Delete  MouseWheel:Forward/Backward  Space:Play/Stop");
		text->SetPosition({ viewport.Width / 16.f, viewport.Height - 25.f });
		text->SetScale(0.25f);
		text->SetColor(DirectX::Colors::Black);

		// 見やすくするためのフレーム
		std::shared_ptr<Framework::UIObject> frame = UIObjectManager::CreateObject();
		Sprite* sprite = new Sprite(L"res/texture/notes_edit_frame.png", SPRITE_PIVOT::TOP_LEFT);
		SpriteRenderer* spriteRenderer = frame->AddComponent<SpriteRenderer>(frame);
		spriteRenderer->SetSprite(sprite);
		spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GUI);
		spriteRenderer->SetLayer(static_cast<UINT>(SPRITE_LAYER::UI));
		Transform2D* transform = frame->GetComponent<Transform2D>();
		transform->position = { 0, viewport.Height - 30.f };
		transform->scale = { viewport.Width, 30.f };

		m_owner.lock()->AddChild(operationText);
		m_owner.lock()->AddChild(frame);
	}
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

		ImGui::SeparatorText("Note Type");
		DrawNoteTypeSelector();

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
		// "res/sound"以下の音楽ファイルパスの一覧を取得する
		auto musicFilePaths = GetFilePathsInDirectoryWithExtension(soundPath.string(), ".wav");

		// 音楽ファイルパスの一覧をドロップダウン形式で表示
		if (ImGui::BeginCombo("Music File", m_selectedMusicPath.c_str()))
		{
			for (int i = 0; i < musicFilePaths.size(); i++)
			{
				bool is_selected = (m_selectedMusicPath == musicFilePaths[i]);

				if (ImGui::Selectable(musicFilePaths[i].c_str(), is_selected))
				{
					m_selectedMusicPath = musicFilePaths[i].c_str();
				}

				// 選択された音楽ファイルパスにフォーカスを当てる
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		// 選択された音楽ファイルパスを譜面データに設定
		m_fumenData.musicFilePath = m_selectedMusicPath;

		ImGui::InputFloat("BPM", &m_fumenData.bpm, 0);
		ImGui::InputInt("Beat", &m_fumenData.beat, 0);
	}
	void NotesEditUI::DrawSaveLoadButton()
	{
		static std::string selectedJsonPath = "No Select";     // ドロップダウンで選択されたファイル名
		static std::string openedFumenPath = "No Opened";       // 現在開いているファイル名
		static std::string actionLog = "";                     // 保存や読み込みの結果を表示

		ImGui::Text("Opened Fumen Path: %s", openedFumenPath.c_str());

		// 譜面情報をJson形式で保存
		if (ImGui::Button("Save"))
		{
			std::string savePath = "res\\fumen\\" + m_fumenData.musicName + ".json";
			OnSave.Notify(savePath, m_fumenData);

			// 保存した譜面ファイルのパスを表示
			openedFumenPath = savePath;
			actionLog = "Saved";
		}
		ImGui::SameLine();

		// 譜面情報をJson形式で読み込み
		if (ImGui::Button("Load"))
		{
			OnLoad.Notify(selectedJsonPath, m_fumenData);

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
			// 曲を再生
			OnPlay.Notify(NotificationEvent());
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop"))
		{
			// 曲を停止
			OnStop.Notify(NotificationEvent());
		}
		ImGui::SameLine();
		if (ImGui::Button("Restart"))
		{
			// 曲を最初から再生
			OnRestart.Notify(NotificationEvent());
		}
	}
	void NotesEditUI::DrawEditStartButton()
	{
		if (ImGui::Button("Edit Start"))
		{
			// 編集開始
			OnEditStart.Notify(m_fumenData);
		}
	}
	void NotesEditUI::DrawNoteTypeSelector()
	{
		ImGui::Text("Note Type");
		if (ImGui::RadioButton("Obstacle", &m_selectedNoteType, static_cast<int>(NoteType::OBSTACLE)))
		{
			OnNoteTypeChanged.Notify(NoteType::OBSTACLE);
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Hidden", &m_selectedNoteType, static_cast<int>(NoteType::HIDDEN)))
		{
			OnNoteTypeChanged.Notify(NoteType::HIDDEN);
		}
	}
}
