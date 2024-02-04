#include "pch.h"
#include "NotesEditor.h"
#include "NotesEditUI.h"

#include "FumenJsonReadWriter.h"
#include "BarManager.h"
#include "NotesManager.h"
#include "MusicPlayer.h"

#include "Obstacle.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace DX12Wrapper;
using namespace Framework;

namespace FlappyBird
{
	NotesEditor::NotesEditor(Framework::Object* owner) : IComponent(owner)
	{
		// 障害物の取得
		GameObject* obstaceleObj = new GameObject();
		obstaceleObj->SetName("Obstacle");
		obstaceleObj->SetActive(false);
		m_obstacle = obstaceleObj->AddComponent<Obstacle>(obstaceleObj);
		m_obstacle->SetMaterialColor({ 1.f, 1.f, 1.f, 0.8f });
		m_owner->AddChild(obstaceleObj);

		float judgeLineX = UIObjectManager::FindObject("JudgeLine")->GetComponent<Transform2D>()->position.x;
		Obstacle::SetJudgeLineX(judgeLineX);
	}
	NotesEditor::~NotesEditor()
	{
	}
	void NotesEditor::Start()
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
		// 障害物の位置を更新
		Obstacle::SetCurrentPlayTime(m_musicPlayer->GetCurrentPlayTimeMs());

		auto viewport = Dx12GraphicsEngine::GetViewport();
		POINT mousePos = InputSystem::GetMousePosition();

		// マウス座標がGameWindow内であればマウスクリックでの処理を行う
		if (IsInsideViewport(mousePos, viewport))
		{
			float timing = CalcNotesTiming(mousePos.x, viewport.Width);

			// 設置用のオブジェクト描画
			m_obstacle->SetTiming(timing);
			m_obstacle->SetPosY(static_cast<float>(mousePos.y));
			m_obstacle->GetOwner()->SetActive(true);

			// マウスクリック時の処理
			if (InputSystem::GetMouseButtonDown(MOUSECODE::LEFT))
			{
				PutNotes(timing, static_cast<float>(mousePos.y));
			}
			if (InputSystem::GetMouseButtonDown(MOUSECODE::RIGHT))
			{
				DeleteNotes(timing, static_cast<float>(mousePos.y));
			}
		}
		else
		{
			m_obstacle->GetOwner()->SetActive(false);
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
	}
	void NotesEditor::PutNotes(float timing, float posY)
	{
		m_notesManager->CreateNotes(Note(timing, posY));
	}
	void NotesEditor::DeleteNotes(float timing, float posY)
	{
		m_notesManager->DeleteNotes(Note(timing, posY));
	}
	float NotesEditor::CalcNotesTiming(LONG mouseX, float viewportWidth)
	{
		// マウス座標からタイミングを計算
		float currentTime = m_musicPlayer->GetCurrentPlayTimeMs();
		float judgeLineX = UIObjectManager::FindObject("JudgeLine")->GetComponent<Transform2D>()->position.x;
		float distanceX = viewportWidth - judgeLineX;
		float posX = (mouseX - judgeLineX) * 2.f;
		float timing = posX / distanceX * 1000.f + currentTime;

		// 最も近い小節線のタイミングを取得
		return m_barManager->GetNearBarLineTiming(timing);
	}
	bool NotesEditor::IsInsideViewport(POINT mousePos, CD3DX12_VIEWPORT viewport)
	{
		// GameWindow内でのみノーツを設置できるようにする
		if (viewport.TopLeftX < mousePos.x && mousePos.x < viewport.Width && // x座標
			viewport.TopLeftY < mousePos.y && mousePos.y < viewport.Height)  // y座標
		{
			return true;
		}

		return false;
	}
}