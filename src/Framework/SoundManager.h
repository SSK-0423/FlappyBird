#pragma once
#include <xaudio2.h>
#include "WAVFileReader.h"

namespace Framework
{
	/// <summary>
	/// サウンドの生データを保持する構造体
	/// </summary>
	struct SoundData
	{
		std::unique_ptr<uint8_t[]> waveFile;
		DirectX::WAVData waveData;
		XAUDIO2_BUFFER buffer;
	};

	// XAudio2Bufferを保持する
	// SouncClipから再生を要求されたら、SourceVoiceを生成して再生する

	/// <summary>
	/// サウンド再生に必要なデータを管理・生成するクラス
	/// </summary>
	class SoundManager
	{
	public:
		static Utility::RESULT Init();
		static void DebugDraw();
		static void Reset();
		static void Final();

		/// <summary>
		/// WAV形式のサウンドを読み込む
		/// </summary>
		/// <param name="filename">wavファイルへのパス</param>
		/// <param name="isLoop">ループ再生するかどうか</param>
		/// <returns>RESULT: 成功 FAILED：失敗</returns>
		static Utility::RESULT LoadWavSound(const wchar_t* filename, bool isLoop);

		/// <summary>
		/// 引数で指定されたサウンドを再生するためのSourceVoiceを返す
		/// サウンドが読み込み済みの場合は、利用可能なSouceVoiceを探すor生成して返す
		/// 読み込み済みでない場合は、読み込みを行い、SouceVoiceを生成して返す
		/// </summary>
		/// <param name="soundname">サウンド名(wavファイルへのパス)</param>
		/// <param name="callback">IXAudio2VoiceCallback関数を実行したインスタンスへのポインタ</param>
		/// <returns>読み込み済み：SourceVoice 読み込み済みでない：nullptr</returns>
		static IXAudio2SourceVoice* Play(const wchar_t* soundname, IXAudio2VoiceCallback* callback);
		
		/// <summary>
		/// 引数で指定されたサウンドの名前データを取得する
		/// 読み込み済みのサウンドの場合は、そのデータを返す
		/// 読み込み済みでない場合は、nullptrを返す
		/// </summary>
		/// <param name="soundname">サウンド名(wavファイルへのパス)</param>
		/// <returns>読み込み済み：SoundData, 読み込み済みでない：nullptr</returns>
		static SoundData* GetSoundData(const wchar_t* soundname);

	private:
		SoundManager() = default;
		~SoundManager() = default;

		static IXAudio2* m_xAudio2;
		static IXAudio2MasteringVoice* m_masteringVoice;
		static HRESULT CreateXAudio2();
		static HRESULT CreateMasteringVoice();

		static std::map<std::wstring, SoundData> m_soundDatas;
		static std::map<std::wstring, std::list<IXAudio2SourceVoice*>> m_sourceVoices;
	};
}