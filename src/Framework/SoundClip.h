#pragma once

#include <xaudio2.h>
#include "WAVFileReader.h"
#include "IComponent.h"
#include "Subject.h"
#include "NotificationEvent.h"

namespace Framework
{
	/// <summary>
	/// サウンドの再生・停止などを行うクラス
	/// </summary>
	class SoundClip : public IComponent, IXAudio2VoiceCallback
	{
	public:
		SoundClip(std::shared_ptr<Object> owner);
		~SoundClip();

	public:
		/// <summary>
		/// WAV形式のサウンドを読み込む
		/// </summary>
		/// <param name="filename">wavファイルへのパス</param>
		/// <param name="isLoop">ループ再生するかどうか</param>
		/// <returns>SUCCESS:成功 FAILED：失敗</returns>
		Utility::RESULT LoadWavSound(const wchar_t* filename, bool isLoop = false);

		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		/// <summary>
		/// 読み込んだサウンドを再生する
		/// </summary>
		/// <param name="volume">音量</param>
		/// <param name="startTimeSec">再生開始する位置(負数・サウンドの長さを超える数値はエラーとなる)</param>
		/// <param name="wait">再生が終わるまで待つかどうか</param>
		void Play(float volume = 0.1f, float startTimeSec = 0.f, bool wait = false);

		/// <summary>
		/// 再生を止める
		/// </summary>
		/// <param name="isPause">一時停止かどうか</param>
		void Stop(bool isPause = false);

		/// <summary>
		/// 現在のループの再生が終わったら再生を終了する
		/// </summary>
		void ExitLoop();

		/// <summary>
		/// 音量セット
		/// </summary>
		/// <param name="volume">音量</param>
		void SetVolume(float volume);

		/// <summary>
		/// サウンドの長さを秒単位で取得する
		/// </summary>
		/// <returns>サウンドが読み込まれていない：0.0f それ以外：サウンドの長さ(秒)</returns>
		float GetLength();

		/// <summary>
		/// 現在の再生位置をミリ秒単位で取得する
		/// </summary>
		/// <returns>サウンドが読み込まれていない：-1.0f それ以外：現在の再生位置(ミリ秒)</returns>
		float GetCurrentPlayTime();

		/// <summary>
		/// サウンドの再生位置を指定秒動かす
		/// </summary>
		/// <param name="timeMs">何秒分動かすか</param>
		void Move(float timeMs);

		/// <summary>
		/// 読み込んだサウンドの再生が終了しているかどうか
		/// </summary>
		/// <returns>true:終了 false：まだ</returns>
		bool IsEnd();

		// IXAudio2VoiceCallback
		// 音声のバッファーキューからデータを読み取る直前に呼ばれる
		STDMETHOD_(void, OnVoiceProcessingPassStart)(UINT32 BytesRequired) override;
		// 音声の処理パスが終了した直後に呼ばれる
		STDMETHOD_(void, OnVoiceProcessingPassEnd)() override;
		// 音声ストリームの再生が終了したときに呼ばれる
		STDMETHOD_(void, OnStreamEnd)() override;
		// 音声がバッファーの処理を開始しようとしているときに呼ばれる
		STDMETHOD_(void, OnBufferStart)(void* pBufferContext) override;
		// 音声がバッファーの処理を終了したときに呼ばれる
		STDMETHOD_(void, OnBufferEnd)(void* pBufferContext) override;
		// 音声がループの終了位置に達したときに呼ばれる
		STDMETHOD_(void, OnLoopEnd)(void* pBufferContext) override;
		// 音声処理中にエラーが発生したときに呼ばれる
		STDMETHOD_(void, OnVoiceError)(void* pBufferContext, HRESULT Error) override;

		Subject<NotificationEvent> OnEnd;

	private:
		const wchar_t* m_soundname;
		IXAudio2SourceVoice* m_sourceVoice;

		INT64 m_samplesPlayed;	    // Moveも考慮した再生済みサンプル数　再生位置の指定にはこの数値を用いる
		UINT64 m_totalSamples;	    // サウンドの総サンプル数
		INT64 m_startSamplesPlayed;	// Playが呼ばれた時のXAUDIO2_VOICE_STATEから取得したサンプル数
		bool m_isPaused;	        // 一時停止中かどうか
		bool m_isEnd;	            // 再生が終了しているかどうか
	};
}