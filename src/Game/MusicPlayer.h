#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	/// <summary>
	/// 曲の再生を管理するコンポーネント
	/// </summary>
	class MusicPlayer : public Framework::IComponent
	{
	public:
		MusicPlayer(std::shared_ptr<Framework::Object> owner);
		~MusicPlayer();

		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		/// <summary>
		/// 曲を読み込む
		/// </summary>
		/// <param name="musicPath">曲のファイルパス</param>
		/// <param name="isLoop">ループ再生するかどうか</param>
		void Load(const std::string& musicPath, bool isLoop);
		void Play(float startTime = 0.f);
		void Stop();

		/// <summary>
		/// 曲の長さを秒単位で取得する
		/// </summary>
		/// <returns>曲の長さ(秒)</returns>
		float GetMusicLength();

		/// <summary>
		/// 現在の再生時間をミリ秒単位で取得する
		/// </summary>
		/// <returns>現在の再生時間</returns>
		float GetCurrentPlayTimeMs();

		/// <summary>
		/// 曲の再生位置を動かす
		/// </summary>
		/// <param name="timeSec">何秒分動かすか</param>
		void Move(float timeSec);

		bool IsPlaying();

		void SetVolume(float volume);
		
		void SetBPM(float bpm);
		float GetBPM();

		/// <summary>
		/// 曲の拍子を設定する
		/// </summary>
		/// <param name="beat"></param>
		void SetBeat(float beat);

		/// <summary>
		/// 曲の拍子を取得する
		/// </summary>
		/// <returns></returns>
		float GetBeat();

		Framework::Subject<Framework::NotificationEvent> OnMusicEnd;	// 曲が再生が終了したときに通知するイベント

	private:
		Framework::SoundClip* m_music;
		wchar_t m_musicPath[256];
		bool m_isPlaying;
		float m_beat;
		float m_bpm;
	};
}