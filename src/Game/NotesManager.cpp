#include "pch.h"
#include "NotesManager.h"

#include "Player.h"
#include "Obstacle.h"
#include "MusicPlayer.h"
#include "TimingCalculator.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

#include "imgui.h"

using namespace Framework;
using namespace DX12Wrapper;

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

		// 現在のシーンを取得
		std::string currentScene = SceneManager::GetCurrentSceneName();
		if (currentScene == "Game")
		{
			// プレイヤーのX座標を判定ラインとして設定
			m_judgeLineX = GameObjectManager::FindObject("Player")->GetComponent<Transform2D>()->position.x;
		}
		else if (currentScene == "NotesEdit")
		{
			// エディターのX座標を判定ラインとして設定
			m_judgeLineX = UIObjectManager::FindObject("JudgeLine")->GetComponent<Transform2D>()->position.x;
		}
		Obstacle::SetJudgeLineX(m_judgeLineX);
	}
	void NotesManager::Update(float deltaTime)
	{
		// 現在の再生位置を更新
		UpdateCurrentPlayTime();

		// ノーツのアクティブ状態を更新
		UpdateNoteActive();

		// ノーツのSEを再生
		PlayNoteSound();
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
		// 既に同じタイミングのノーツが存在する場合は追加しない
		for (auto& note : m_notes)
		{
			if (note.timing == data.timing)
			{
				return;
			}
		}
		m_notes.push_back(data);

		// ノーツの生成
		std::shared_ptr<GameObject> noteObstacle = std::shared_ptr<GameObject>(new GameObject());
		noteObstacle->SetName("NoteObstacle");
		noteObstacle->SetActive(false);
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
		// 既存のノーツを削除
		m_notes.clear();
		m_notes.shrink_to_fit();
		m_owner.lock()->RemoveAllChildren();
		m_noteObstacles.clear();
		m_noteObstacles.shrink_to_fit();

		for (auto& note : notes)
		{
			CreateNotes(note);
		}

		Editor::DebugLog("Notes Set Completed");
	}
	void NotesManager::UpdateCurrentPlayTime()
	{
		if (m_musicPlayer != nullptr)
		{
			// 小節線に現在の再生位置をセット
			Obstacle::SetCurrentPlayTime(m_musicPlayer->GetCurrentPlayTimeMs());
		}
	}
	void NotesManager::UpdateNoteActive()
	{
		// 現在の再生位置を取得
		float currentPlayTime = m_musicPlayer->GetCurrentPlayTimeMs();

		auto& viewport = Dx12GraphicsEngine::GetViewport();

		// ビューポートの左端と右端のタイミングを計算
		float viewportLeftTime = TimingCalculator::CalcTiming(m_judgeLineX, viewport.TopLeftX, viewport.Width, currentPlayTime);
		float viewportRightTime = TimingCalculator::CalcTiming(m_judgeLineX, viewport.TopLeftX + viewport.Width, viewport.Width, currentPlayTime);

		// ノーツがアクティブ・非アクティブになる瞬間を見せないようにするために、
		// ビューポートの左端と右端のタイミングを少し広げる
		viewportLeftTime -= TIMING_OFFSET;
		viewportRightTime += TIMING_OFFSET;

		// ノーツのアクティブ状態を更新
		for (auto note : m_noteObstacles)
		{
			float noteTiming = note->GetTiming();

			if (viewportLeftTime <= noteTiming && noteTiming <= viewportRightTime)
			{
				note->GetOwner()->SetActive(true);
			}
			else
			{
				note->Reset();
				note->GetOwner()->SetActive(false);
			}
		}
	}
	void NotesManager::PlayNoteSound()
	{
		float currentPlayTime = m_musicPlayer->GetCurrentPlayTimeMs();

		for (auto note : m_noteObstacles)
		{
			// ノーツのタイミングと判定ラインのタイミングの差を計算
			float diff = note->GetTiming() - currentPlayTime;

			// 判定タイミングを超えたらSEを再生
			if (std::abs(diff) <= 16.f)
			{
				// 以下の条件に合致する場合、SEを再生
				// アクティブである
				// かつ、SEが再生可能である
				if (note->GetOwner()->GetActive() &&
					note->CanPlaySE())
				{
					m_owner.lock()->GetComponent<SoundClip>()->Play(0.5f);
					note->SetCanPlaySE(false);	// 複数回再生できないようにする
				}
			}
		}
	}
}