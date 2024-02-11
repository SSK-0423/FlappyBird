#include "pch.h"
#include "GameMaster.h"
#include "Player.h"
#include "GameOverUI.h"
#include "GameReadyUI.h"

#include "FumenJsonReadWriter.h"
#include "NotesManager.h"
#include "HiddenNotesManager.h"
#include "MusicPlayer.h"

#include "MusicSelect.h"
using namespace Framework;

namespace FlappyBird
{
	GameMaster::GameMaster(std::shared_ptr<Framework::Object> owner) :
		IComponent(owner), m_gameState(GAME_STATE::READY), m_elapsedTime(0.0f), m_gameStartTime(4.0f)
	{
		m_gameOverUI = UIObjectManager::FindObject("GameOverUI");
		m_gameReadyUI = UIObjectManager::FindObject("GameReadyUI");
		m_gameClearUI = UIObjectManager::FindObject("GameClearUI");

		m_owner.lock()->SetName("GameMaster");
	}
	void GameMaster::Start()
	{
		// 譜面読み込み
		FumenData fumenData = MusicSelect::selectedFumenData;

		// ノーツの生成
		std::shared_ptr<Framework::GameObject> notesManagerObj = GameObjectManager::FindObject("NotesManager");
		notesManagerObj->GetComponent<NotesManager>()->SetNotes(fumenData.noteDatas);

		// 曲読み込み
		std::shared_ptr<Framework::GameObject> musicPlayerObj = GameObjectManager::FindObject("MusicPlayer");
		MusicPlayer* musicPlayer = musicPlayerObj->GetComponent<MusicPlayer>();
		musicPlayer->Load(fumenData.musicFilePath, false);
		musicPlayer->SetBPM(fumenData.bpm);
		musicPlayer->SetBeat(fumenData.beat);
		musicPlayer->OnMusicEnd.Subscribe([this](NotificationEvent e) { ChangeState(GAME_STATE::GAMECLEAR); });

		// 2小節分ジャンプアニメーションの間隔を計算
		float bpm = musicPlayer->GetBPM();
		float beat = musicPlayer->GetBeat();

		// 4分音符の間隔を計算
		float barTimeLength = 60.f / bpm * beat;
		m_gameStartTime = barTimeLength * 1.f;

		// 隠しノーツの生成
		float musicLength = musicPlayer->GetMusicLength();
		unsigned int barNum = musicLength * fumenData.bpm / (60.f * fumenData.beat);
		std::shared_ptr<Framework::GameObject> hiddenNotesManagerObj = GameObjectManager::FindObject("HiddenNotesManager");
		hiddenNotesManagerObj->GetComponent<HiddenNotesManager>()->CreateHiddenNotes(barNum, fumenData.bpm, fumenData.beat);

	}
	void GameMaster::Update(float deltaTime)
	{
		switch (m_gameState)
		{
		case GAME_STATE::READY:
			GameReady(deltaTime);
			break;
		case GAME_STATE::PLAYING:
			break;
		case GAME_STATE::GAMEOVER:
			GameOver(deltaTime);
			break;
		default:
			break;
		}
	}
	void GameMaster::Draw()
	{
	}
	void GameMaster::ChangeState(GAME_STATE state)
	{
		m_gameState = state;

		// ステート切り替え時の処理
		switch (m_gameState)
		{
		case GAME_STATE::READY:
			OnGameReady();
			break;
		case GAME_STATE::PLAYING:
			OnGameStart();
			break;
		case GAME_STATE::GAMECLEAR:
			OnGameClear();
			break;
		case GAME_STATE::GAMEOVER:
			OnGameOver();
			break;
		default:
			break;
		}
	}
	GAME_STATE GameMaster::GetGameState()
	{
		return m_gameState;
	}
	void GameMaster::GameReady(float deltaTime)
	{
		m_elapsedTime += deltaTime;
		if (m_elapsedTime >= m_gameStartTime)
		{
			// コールバックを呼ぶために直接代入ではなく関数を呼ぶ
			ChangeState(GAME_STATE::PLAYING);
		}
	}
	void GameMaster::GameOver(float deltaTime)
	{
		// ゲームオーバーUIを表示
	}
	void GameMaster::OnGameReady()
	{
		// ゲーム開始UIを表示
		m_gameReadyUI->SetActive(true);
	}
	void GameMaster::OnGameStart()
	{
		// ゲーム開始UIを非表示
		m_gameReadyUI->SetActive(false);

		// 曲の再生を開始
		GameObjectManager::FindObject("MusicPlayer")->GetComponent<MusicPlayer>()->Play();
	}
	void GameMaster::OnGameClear()
	{
		// ゲームクリアUIを表示
		m_gameClearUI->SetActive(true);

		// 曲の再生を停止
		GameObjectManager::FindObject("MusicPlayer")->GetComponent<MusicPlayer>()->Stop();

		// ノーツの描画を停止
		GameObjectManager::FindObject("NotesManager")->GetComponent<NotesManager>()->SetActive(false);
	}
	void GameMaster::OnGameOver()
	{
		// ゲームオーバーUIを表示
		m_gameOverUI->SetActive(true);

		// 曲の再生を停止
		GameObjectManager::FindObject("MusicPlayer")->GetComponent<MusicPlayer>()->Stop();

		// ノーツの移動を停止
		GameObjectManager::FindObject("NotesManager")->GetComponent<NotesManager>()->SetActive(false);
	}
}