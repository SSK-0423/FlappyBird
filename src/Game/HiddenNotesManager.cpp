#include "pch.h"
#include "HiddenNotesManager.h"

#include "JumpPoint.h"
#include "MusicPlayer.h"
#include "TimingCalculator.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace DX12Wrapper;
using namespace Framework;

namespace FlappyBird
{
	HiddenNotesManager::HiddenNotesManager(std::shared_ptr<Framework::Object> owner)
		: Framework::IComponent(owner)
	{
		// SE追加
		m_seClip = m_owner.lock()->AddComponent<SoundClip>(m_owner.lock());
		m_seClip->LoadWavSound(L"res/sound/jump.wav");

		// 判定結果イベント登録
		OnJudge.Subscribe(std::bind(&HiddenNotesManager::JudgeEvent, this, std::placeholders::_1));
	}

	void HiddenNotesManager::Start()
	{
		// 曲プレイヤー取得
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
		JumpPoint::SetJudgeLineX(m_judgeLineX);
	}

	void HiddenNotesManager::Update(float deltaTime)
	{
		// GameSceneなら隠しノーツの判定を行う
		if (SceneManager::GetCurrentSceneName() == "Game")
		{
			// ジャンプしたら隠しノーツの判定を行う
			if (InputSystem::GetMouseButtonDown(MOUSECODE::LEFT))
			{
				JudgeTiming();
			}
		}

		//else if (SceneManager::GetCurrentSceneName() == "NotesEdit")
		//{
		//}

		// エディターなら隠しノーツのアクティブ状態を更新
		UpdateHiddenNotesActive();

		// 現在の再生位置を更新
		UpdateCurrentPlayTime();

		// 隠しノーツのアクティブ状態を更新
		UpdateHiddenNotesState();
	}

	void HiddenNotesManager::Draw()
	{
	}
	void HiddenNotesManager::DrawInspector()
	{
		if (ImGui::CollapsingHeader("HiddenNotesManager"))
		{
			for (auto& hiddenNote : m_hiddenNotes)
			{
				ImGui::Text("Timing: %f", hiddenNote.timing);
				ImGui::Text("IsJudged: %s", hiddenNote.isJudged ? "true" : "false");
			}
		}
	}
	bool HiddenNotesManager::CreateHiddenNotes(HiddenNoteData data)
	{
		// 既に同じタイミングの隠しノーツが存在する場合は追加しない
		for (auto& hiddenNote : m_hiddenNotes)
		{
			if (hiddenNote.timing == data.timing)
			{
				return false;
			}
		}
		m_hiddenNotes.push_back(data);

		// 隠しノーツの生成
		std::shared_ptr<GameObject> hiddenNoteJumpPoint = std::shared_ptr<GameObject>(new GameObject());
		hiddenNoteJumpPoint->SetName("HiddenNoteJumpPoint");
		hiddenNoteJumpPoint->SetActive(false);
		JumpPoint* jumpPoint = hiddenNoteJumpPoint->AddComponent<JumpPoint>(hiddenNoteJumpPoint);
		jumpPoint->SetTiming(data.timing);
		m_jumpPoints.push_back(jumpPoint);

		m_owner.lock()->AddChild(hiddenNoteJumpPoint);

		return true;
	}
	bool HiddenNotesManager::DeleteHiddenNotes(HiddenNoteData data)
	{
		bool couldDeleteData = false;
		bool couldDeleteNote = false;

		// 隠しノーツデータを削除
		for (auto it = m_hiddenNotes.begin(); it != m_hiddenNotes.end(); ++it)
		{
			if (it->timing == data.timing)
			{
				m_hiddenNotes.erase(it);
				couldDeleteData = true;
				break;
			}
		}

		// 隠しノーツを削除
		for (auto it = m_jumpPoints.begin(); it != m_jumpPoints.end(); ++it)
		{
			if ((*it)->GetTiming() == data.timing)
			{
				m_owner.lock()->RemoveChild((*it)->GetOwner());
				m_jumpPoints.erase(it);
				couldDeleteNote = true;
				break;
			}
		}

		return couldDeleteData && couldDeleteNote;
	}
	void HiddenNotesManager::SetHiddenNotes(const std::vector<HiddenNoteData>& hiddenNotes)
	{
		// 既存の隠しノーツを削除
		m_hiddenNotes.clear();
		m_hiddenNotes.shrink_to_fit();
		m_jumpPoints.clear();
		m_jumpPoints.shrink_to_fit();

		// 与えられた情報を元に隠しノーツ生成
		for (auto& hiddenNote : hiddenNotes)
		{
			CreateHiddenNotes(hiddenNote);
		}
	}
	std::vector<HiddenNoteData>& HiddenNotesManager::GetHiddenNotes()
	{
		return m_hiddenNotes;
	}
	void HiddenNotesManager::UpdateHiddenNotesState()
	{
		// 隠しノーツの判定状況を管理する
		float currentPlayTime = m_musicPlayer->GetCurrentPlayTimeMs();

		for (size_t i = m_currentHiddenNoteIndex; i < m_hiddenNotes.size(); i++)
		{
			auto& hiddenNote = m_hiddenNotes[i];

			if (hiddenNote.isJudged) continue;

			// 曲の現在の再生位置と隠しノーツのタイミングの差を計算
			float diff = currentPlayTime - hiddenNote.timing;

			// ノーツが判定ラインを通過したかを以下の条件で判定
			// 差分が判定範囲より大きい場合、ノーツは判定ラインを通過している
			if (diff > JudgeRange::GOOD_JUDGE_RANGE)
			{
				hiddenNote.isJudged = true;
				m_currentHiddenNoteIndex = i;
			}
		}
	}
	void HiddenNotesManager::JudgeTiming()
	{
		float jumpTiming = m_musicPlayer->GetCurrentPlayTimeMs();

		// JumpTimingに最も近い隠しノーツを探す
		float minDiff = FLT_MAX;
		size_t nearestNoteIndex = 0;

		for (size_t i = m_currentHiddenNoteIndex; i < m_hiddenNotes.size(); i++)
		{
			auto& hiddenNote = m_hiddenNotes[i];

			// 既に判定済みのノーツは判定しない
			if (hiddenNote.isJudged) continue;

			float timingDiff = std::abs(hiddenNote.timing - jumpTiming);

			if (timingDiff < minDiff)
			{
				minDiff = timingDiff;
				nearestNoteIndex = i;
			}
		}

#ifdef _DEBUG
		//Editor::DebugLog("Jump Timing: %f", jumpTiming);
		//Editor::DebugLog("NearestNote Timing: %f", nearestNote.timing);
		//Editor::DebugLog("MinDiff: %f", minDiff);
#endif // _DEBUG

		// 判定範囲内であれば判定結果を表示
		if (minDiff <= JudgeRange::PERFECT_JUDGE_RANGE)
		{
			m_hiddenNotes[nearestNoteIndex].isJudged = true;
			OnJudge.Notify(JUDGE_RESULT::PERFECT);
		}
		else if (minDiff <= JudgeRange::GREAT_JUDGE_RANGE)
		{
			m_hiddenNotes[nearestNoteIndex].isJudged = true;
			OnJudge.Notify(JUDGE_RESULT::GREAT);
		}
		else if (minDiff <= JudgeRange::GOOD_JUDGE_RANGE)
		{
			m_hiddenNotes[nearestNoteIndex].isJudged = true;
			OnJudge.Notify(JUDGE_RESULT::GOOD);
		}
	}
	void HiddenNotesManager::UpdateCurrentPlayTime()
	{
		if (m_musicPlayer != nullptr)
		{
			// 現在の再生位置をセット
			JumpPoint::SetCurrentPlayTime(m_musicPlayer->GetCurrentPlayTimeMs());
		}
	}
	void HiddenNotesManager::JudgeEvent(JUDGE_RESULT result)
	{
		// 判定結果に応じた処理を行う
		switch (result)
		{
		case JUDGE_RESULT::PERFECT:
			Editor::DebugLog("Perfect!");
			break;
		case JUDGE_RESULT::GREAT:
			Editor::DebugLog("Great!");
			break;
		case JUDGE_RESULT::GOOD:
			Editor::DebugLog("Good!");
			break;
		default:
			break;
		}

		// SE再生
		m_seClip->Play(0.5f);
	}
	void HiddenNotesManager::UpdateHiddenNotesActive()
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
		for (auto note : m_jumpPoints)
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
}