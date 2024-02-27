#include "pch.h"
#include "NotesEditor.h"
#include "NotesEditUI.h"

#include "FumenJsonReadWriter.h"
#include "BarManager.h"
#include "NotesManager.h"
#include "HiddenNotesManager.h"
#include "MusicPlayer.h"

#include "Obstacle.h"
#include "JumpPoint.h"
#include "TimingCalculator.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace DX12Wrapper;
using namespace Framework;

namespace FlappyBird
{
	NotesEditor::NotesEditor(std::shared_ptr<Object> owner) : IComponent(owner)
	{
		// 半透明で描画される障害物を生成
		std::shared_ptr<GameObject> obstaceleObj = std::shared_ptr<GameObject>(new GameObject());
		obstaceleObj->SetName("Obstacle");
		obstaceleObj->SetActive(false);
		m_obstacle = obstaceleObj->AddComponent<Obstacle>(obstaceleObj);
		m_obstacle->SetMaterialColor({ 1.f, 1.f, 1.f, 0.8f });
		m_owner.lock()->AddChild(obstaceleObj);

		// 半透明で描画されるジャンプポイントを生成
		std::shared_ptr<GameObject> jumpPointObj = std::shared_ptr<GameObject>(new GameObject());
		jumpPointObj->SetName("JumpPoint");
		jumpPointObj->SetActive(false);
		m_jumpPoint = jumpPointObj->AddComponent<JumpPoint>(jumpPointObj);
		Material* material = jumpPointObj->AddComponent<Material>(jumpPointObj);
		material->SetColor({ 1.f, 1.f, 1.f, 0.8f });
		m_owner.lock()->AddChild(jumpPointObj);

		// ノーツ設置系のSEオブジェクト追加
		// ノーツ設置時SE
		std::shared_ptr<GameObject> putNotesSoundObj = std::shared_ptr<GameObject>(new GameObject());
		putNotesSoundObj->SetName("PutNotesSound");
		m_putNotesSound = putNotesSoundObj->AddComponent<SoundClip>(putNotesSoundObj);
		m_putNotesSound->LoadWavSound(L"res/sound/put_notes.wav");
		// ノーツ削除時SE
		std::shared_ptr<GameObject> deleteNotesSoundObj = std::shared_ptr<GameObject>(new GameObject());
		deleteNotesSoundObj->SetName("DeleteNotesSound");
		m_deleteNotesSound = deleteNotesSoundObj->AddComponent<SoundClip>(deleteNotesSoundObj);
		m_deleteNotesSound->LoadWavSound(L"res/sound/delete_notes.wav");
		// ノーツが設置できなかった時のSE
		std::shared_ptr<GameObject> cannotPutNotsSoundObj = std::shared_ptr<GameObject>(new GameObject());
		cannotPutNotsSoundObj->SetName("PutNotesFailedSound");
		m_cannotPutNotesSound = cannotPutNotsSoundObj->AddComponent<SoundClip>(cannotPutNotsSoundObj);
		m_cannotPutNotesSound->LoadWavSound(L"res/sound/cannot_put_notes.wav");

		// 子オブジェクトに追加
		m_owner.lock()->AddChild(putNotesSoundObj);
		m_owner.lock()->AddChild(deleteNotesSoundObj);
		m_owner.lock()->AddChild(cannotPutNotsSoundObj);
	}
	NotesEditor::~NotesEditor()
	{
	}
	void NotesEditor::Start()
	{
		// UIの各種ボタン操作時の処理を登録
		NotesEditUI* notesEditUI = UIObjectManager::FindObject("NotesEditUI")->GetComponent<NotesEditUI>();

		notesEditUI->OnSave.Subscribe([this](const std::string& savePath, FumenData& data)
			{
				// ノーツデータを取得
				SaveFumen(savePath, data);
			});

		notesEditUI->OnLoad.Subscribe([this](const std::string& loadPath, FumenData& data)
			{
				// 譜面を読み込んで編集開始
				LoadFumen(loadPath, data);
				StartEdit(data);
			});

		notesEditUI->OnPlay.Subscribe([this](NotificationEvent e) { Play(); });
		notesEditUI->OnStop.Subscribe([this](NotificationEvent e) { Stop(); });
		notesEditUI->OnRestart.Subscribe([this](NotificationEvent e) { Restart(); });
		notesEditUI->OnEditStart.Subscribe([this](const FumenData& data)
			{
				// 以前編集中だった場合はノーツを全削除
				m_notesManager->DeleteAllNotes();
				StartEdit(data);
			});
		notesEditUI->OnNoteTypeChanged.Subscribe([this](NoteType noteType)
			{
				// 設置するノーツの種類を変更
				m_putNoteType = noteType;
			});

		// 各種コンポーネントの取得
		m_barManager = UIObjectManager::FindObject("BarManager")->GetComponent<BarManager>();
		m_notesManager = GameObjectManager::FindObject("NotesManager")->GetComponent<NotesManager>();
		m_hiddenNotesManager = GameObjectManager::FindObject("HiddenNotesManager")->GetComponent<HiddenNotesManager>();
		m_musicPlayer = GameObjectManager::FindObject("MusicPlayer")->GetComponent<MusicPlayer>();

		// 判定ラインの位置を取得
		m_judgeLineX = UIObjectManager::FindObject("JudgeLine")->GetComponent<Transform2D>()->position.x;
	}
	void NotesEditor::Update(float deltaTime)
	{
		// まだ編集が開始されていない場合は処理を行わない
		if (!m_isStartedEdit)
		{
			return;
		}

#ifdef _DEBUG
#else
		// 編集ウィンドウ上にマウスがあるかどうかを判定
		if (ImGui::GetIO().WantCaptureMouse)
		{
			return;
		}
#endif // _DEBUG

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
			m_obstacle->SetSpaceOffset(m_spaceOffset);
			m_obstacle->SetPosY(static_cast<float>(mousePos.y));

			m_jumpPoint->SetTiming(timing);

			// ノーツの種類によって障害物とジャンプポイントを切り替える
			switch (m_putNoteType)
			{
			case FlappyBird::NoteType::OBSTACLE:
				m_obstacle->GetOwner()->SetActive(true);
				m_jumpPoint->GetOwner()->SetActive(false);
				break;
			case FlappyBird::NoteType::HIDDEN:
				m_obstacle->GetOwner()->SetActive(false);
				m_jumpPoint->GetOwner()->SetActive(true);
				break;
			default:
				break;
			}

			// マウスクリック時の処理
			if (InputSystem::GetMouseButtonDown(MOUSECODE::LEFT))
			{
				PutNotes(m_putNoteType, timing, static_cast<float>(mousePos.y), m_spaceOffset);
			}
			if (InputSystem::GetMouseButtonDown(MOUSECODE::RIGHT))
			{
				DeleteNotes(m_putNoteType, timing);
			}

			// 曲が再生中でなければマウスホイールでの処理を行う
			if (!m_musicPlayer->IsPlaying())
			{
				// マウスホイールで曲を進める
				Scroll(InputSystem::GetMouseWheelMovement());
			}

			// 数字キーで上下の土管の間のスペースを調整
			if (InputSystem::GetKey(DIK_1))
			{
				m_spaceOffset -= 5.f;
			}
			if (InputSystem::GetKey(DIK_2))
			{
				m_spaceOffset += 5.f;
			}

			// スペースキーで曲の再生・停止を切り替える
			if (InputSystem::GetKeyDown(DIK_SPACE))
			{
				if (m_musicPlayer->IsPlaying())
				{
					Stop();
				}
				else
				{
					Play();
				}
			}
		}
		else
		{
			m_obstacle->GetOwner()->SetActive(false);
			m_jumpPoint->GetOwner()->SetActive(false);
		}
	}
	void NotesEditor::Draw()
	{
	}
	void NotesEditor::SaveFumen(const std::string& savePath, FumenData& data)
	{
		// ノーツデータを書き込み
		data.noteDatas = m_notesManager->GetNotes();
		data.hiddenNoteDatas = m_hiddenNotesManager->GetHiddenNotes();

		// Json形式で保存
		FumenJsonReadWriter::Write(savePath, data);
	}
	void NotesEditor::LoadFumen(const std::string& loadPath, FumenData& data)
	{
		// Json形式で読み込み
		FumenJsonReadWriter::Read(loadPath, data);

		// ノーツデータを書き込み
		m_notesManager->SetNotes(data.noteDatas);
		m_hiddenNotesManager->SetHiddenNotes(data.hiddenNoteDatas);
	}
	void NotesEditor::Play()
	{
		// 判定ラインの位置のタイミングを計算
		float currentPlayTime = m_musicPlayer->GetCurrentPlayTimeMs();
		float viewportWidth = Dx12GraphicsEngine::GetViewport().Width;
		float timing = TimingCalculator::CalcTiming(m_judgeLineX, m_judgeLineX, viewportWidth, currentPlayTime);

		// 再生開始時間を設定
		// 負数や曲の長さを超える数値で再生するとエラーが発生するため、範囲を制限
		float startTimeSec = std::clamp(timing / 1000.f, 0.f, m_musicPlayer->GetMusicLength());

		m_notesManager->ResetCanPlaySE();
		m_musicPlayer->Play(startTimeSec);
	}
	void NotesEditor::Stop()
	{
		m_notesManager->ResetCanPlaySE();
		m_musicPlayer->Stop();
	}
	void NotesEditor::Restart()
	{
		m_musicPlayer->Play(0.f);
	}
	void NotesEditor::StartEdit(const FumenData& data)
	{
		m_isStartedEdit = true;

		// 音楽ファイルを読み込み
		m_musicPlayer->Load(data.musicFilePath, false);

		float musicLength = m_musicPlayer->GetMusicLength();

		// 小節数 = 曲の再生時間(sec) * bpm / (60 * beat)
		unsigned int barNum = musicLength * data.bpm / (60.f * data.beat);

		// 小節線を生成する
		m_barManager->CreateBar(barNum, data.bpm, data.beat);
	}
	void NotesEditor::PutNotes(NoteType type, float timing, float posY, float spaceOffset)
	{
		bool couldCreate = false;

		switch (type)
		{
		case FlappyBird::NoteType::OBSTACLE:
			couldCreate = m_notesManager->CreateNotes(NoteData(timing, posY, spaceOffset));
			break;
		case FlappyBird::NoteType::HIDDEN:
			couldCreate = m_hiddenNotesManager->CreateHiddenNotes(HiddenNoteData(timing));
			break;
		default:
			break;
		}

		// 生成できたら生成時SEを再生
		if (couldCreate)
		{
			m_putNotesSound->Play(0.5f);
		}
		// できなかったら失敗時SEを再生
		else
		{
			m_cannotPutNotesSound->Play(0.5f);
		}
	}
	void NotesEditor::DeleteNotes(NoteType type, float timing)
	{
		bool couldDelete = false;

		switch (type)
		{
		case FlappyBird::NoteType::OBSTACLE:
			couldDelete = m_notesManager->DeleteNotes(timing);
			break;
		case FlappyBird::NoteType::HIDDEN:
			couldDelete = m_hiddenNotesManager->DeleteHiddenNotes(timing);
			break;
		default:
			break;
		}

		// 削除できたら削除時SEを再生
		if (couldDelete)
		{
			m_deleteNotesSound->Play(0.5f);
		}
	}
	void NotesEditor::Scroll(LONG mouseWheelMovement)
	{
		if (mouseWheelMovement > 0)
		{
			// 曲を進める
			m_musicPlayer->Move(0.1f);
		}
		else if (mouseWheelMovement < 0)
		{
			// 曲を戻す
			m_musicPlayer->Move(-0.1f);
		}
	}
	float NotesEditor::CalcNotesTiming(LONG targetPosX, float viewportWidth)
	{
		// ターゲット位置のタイミングを計算
		float timing = TimingCalculator::CalcTiming(m_judgeLineX, targetPosX, viewportWidth, m_musicPlayer->GetCurrentPlayTimeMs());

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