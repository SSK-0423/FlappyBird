#include "pch.h"
#include "NotesEditor.h"
#include "NotesEditUI.h"

#include "FumenJsonReadWriter.h"
#include "BarManager.h"
#include "NotesManager.h"
#include "MusicPlayer.h"

using namespace Framework;

namespace FlappyBird
{
	NotesEditor::NotesEditor(Framework::Object* owner) : IComponent(owner)
	{
	}
	NotesEditor::~NotesEditor()
	{
	}
	void NotesEditor::Init()
	{
		// UIの各種ボタン操作時の処理を登録
		NotesEditUI* notesEditUI = UIObjectManager::FindObject("NotesEditUI")->GetComponent<NotesEditUI>();

		notesEditUI->OnSave.Subscribe([this](const std::string& savePath, const FumenData& data) { SaveFumen(savePath, data); });
		notesEditUI->OnLoad.Subscribe([this](const std::string& loadPath, FumenData& data) { LoadFumen(loadPath, data); });
		notesEditUI->OnPlay.Subscribe([this](NotificationEvent e) { Play(); });
		notesEditUI->OnStop.Subscribe([this](NotificationEvent e) { Stop(); });
		notesEditUI->OnLoadMusic.Subscribe([this](const std::string& musicPath) { LoadMusic(musicPath); });
		notesEditUI->OnEditStart.Subscribe([this](const FumenData& data) { StartEdit(data); });

		// 各種コンポーネントの取得
		m_barManager = UIObjectManager::FindObject("BarManager")->GetComponent<BarManager>();

		m_notesManager = GameObjectManager::FindObject("NotesManager")->GetComponent<NotesManager>();
		m_musicPlayer = GameObjectManager::FindObject("MusicPlayer")->GetComponent<MusicPlayer>();
	}
	void NotesEditor::Update(float deltaTime)
	{
		// マウスクリック時の処理
		if (InputSystem::GetMouseButtonDown(MOUSECODE::LEFT))
		{
			PutNotes();
		}
		if (InputSystem::GetMouseButtonDown(MOUSECODE::RIGHT))
		{
			DeleteNotes();
		}
	}
	void NotesEditor::Draw()
	{
	}
	void NotesEditor::SaveFumen(const std::string& savePath, const FumenData& data)
	{
		// ノーツデータを書き込み

		// Json形式で保存
		FumenJsonReadWriter::Write(savePath, data);
	}
	void NotesEditor::LoadFumen(const std::string& loadPath, FumenData& data)
	{
		// Json形式で読み込み
		FumenJsonReadWriter::Read(loadPath, data);

		// 曲読み込み
		m_musicPlayer->Load(data.musicFilePath.c_str());

		// ノーツデータを書き込み
	}
	void NotesEditor::Play()
	{
		m_musicPlayer->Play();
	}
	void NotesEditor::Stop()
	{
		m_musicPlayer->Stop();
	}
	void NotesEditor::LoadMusic(const std::string& musicPath)
	{
		// 音楽ファイルを読み込み
		m_musicPlayer->Load(musicPath.c_str());
	}
	void NotesEditor::StartEdit(const FumenData& data)
	{
		// 曲の長さを表示
		float musicLength = m_musicPlayer->GetMusicLength();

		// 小節数 = 曲の再生時間(sec) * bpm / (60 * beat)
		unsigned int barNum = std::ceil(musicLength * data.bpm / (60.f * data.beat));

		// 小節線を生成する
		m_barManager->CreateBar(barNum, data.bpm, data.beat);

		Editor::DebugLog("Music Length : %f", musicLength);
		Editor::DebugLog("Bar Count : %ud", barNum);
	}
	void NotesEditor::PutNotes()
	{
		//// ノーツ設置
		//Editor::DebugLog("Put Notes");

		//// ノーツの設置場所は到達時間に合わせる

		//// マウス座標を元に到達時間を取得
		//POINT mousePos = InputSystem::GetMousePosition();

		//// ノーツは時間を元に描画位置を決定する
		//// 1. 
	}
	void NotesEditor::DeleteNotes()
	{
		// ノーツ削除
		//Editor::DebugLog("Delete Notes");
	}
}