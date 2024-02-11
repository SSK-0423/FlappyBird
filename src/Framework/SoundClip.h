#pragma once

#include <xaudio2.h>
#include "WAVFileReader.h"
#include "IComponent.h"
#include "Subject.h"
#include "NotificationEvent.h"

namespace Framework
{
	class SoundClip : public IComponent, IXAudio2VoiceCallback
	{
	public:
		SoundClip(std::shared_ptr<Object> owner);
		~SoundClip();

	public:
		// サウンドデータの設定
		Utility::RESULT LoadWavSound(const wchar_t* filename, bool isLoop = false);

		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		/// <summary>
		/// サウンド再生
		/// </summary>
		/// <param name="wait">音の再生が終了するまで待つか</param>
		void Play(float volume = 0.1f, float startTimeSec = 0.f, bool wait = false);

		void Stop(bool isPause = false);

		void ExitLoop();

		float GetLength();
		float GetCurrentPlayTime();
		void Seek(float timeMs);
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
		UINT64 m_restartSamplesPlayed;
		bool m_isPaused;
		bool m_isEnd;
	};
}