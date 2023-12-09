#include "SoundClip.h"
#include "SoundEngine.h"
#include "WAVFileReader.h"

namespace Framework
{
	SoundClip::SoundClip(Object* owner) : IComponent(owner)
	{
	}
	SoundClip::~SoundClip()
	{
		Utility::DebugLog("SoundClip::~SoundClip()\n");
		m_sourceVoice->DestroyVoice();
	}
	Utility::RESULT SoundClip::LoadWavSound(const wchar_t* filename)
	{
		// WAVファイル読み込み
		if (FAILED(DirectX::LoadWAVAudioFromFileEx(filename, wavData, m_wavData)))
		{
			WCHAR message[1024];
			swprintf_s(message, L"wavファイルの読み込みに失敗しました。対象ファイル： %s", filename);
			MessageBoxW(NULL, message, L"エラー", MB_OK);

			return Utility::RESULT::FAILED;
		}

		// ソースボイス生成
		//IXAudio2SourceVoice* sourceVoice;
		if (FAILED(SoundEngine::XAudio2().CreateSourceVoice(&m_sourceVoice, m_wavData.wfx)))
		{
			WCHAR message[1024];
			swprintf_s(message, L"ソースボイスの生成に失敗しました。対象ファイル： %s", filename);
			MessageBoxW(NULL, message, L"エラー", MB_OK);

			return Utility::RESULT::FAILED;
		}
		//m_sourceVoice.reset(sourceVoice);

		// ソースボイスに音声データをセット
		XAUDIO2_BUFFER buffer = { 0 };
		buffer.pAudioData = m_wavData.startAudio;
		// データの終端を示すフラグ データサイズ分のバッファを再生すると自動的に停止する
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.AudioBytes = m_wavData.audioBytes;

		// TODO:その他ループ再生などの設定

		if (FAILED(m_sourceVoice->SubmitSourceBuffer(&buffer)))
		{
			WCHAR message[1024];
			swprintf_s(message, L"ソースボイスへの音声データのセットに失敗しました。対象ファイル： %s", filename);
			MessageBoxW(NULL, message, L"エラー", MB_OK);

			return Utility::RESULT::FAILED;
		}

		return Utility::RESULT::SUCCESS;
	}
	void SoundClip::Update(float deltaTime)
	{
	}
	void SoundClip::Draw()
	{
	}
	void SoundClip::PlayOneShot(bool wait)
	{
		// 再生中かどうかを判定する必要がある
		// 直ぐにデリートされては困るので、一度再生が終わるまで待つ
		m_sourceVoice->Start();

		// サウンド管理クラスを作成して、再生中かどうかを判定、管理した方がよいのでは
		if (wait)
		{
			XAUDIO2_VOICE_STATE state;
			m_sourceVoice->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
			while (state.BuffersQueued > 0)
			{
				m_sourceVoice->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
			}
		}
	}
	void SoundClip::PlayLoop()
	{
	}
}