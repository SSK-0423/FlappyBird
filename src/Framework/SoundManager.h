#pragma once
#include <xaudio2.h>
#include "WAVFileReader.h"

namespace Framework
{
	struct SoundData
	{
		std::unique_ptr<uint8_t[]> waveFile;
		DirectX::WAVData waveData;
		XAUDIO2_BUFFER buffer;
	};

	// XAudio2Bufferを保持する
	// SouncClipから再生を要求されたら、SourceVoiceを生成して再生する
	class SoundManager
	{
	public:
		static Utility::RESULT Init();
		static void Reset();
		static void Final();
		static Utility::RESULT LoadWavSound(const wchar_t* filename, bool isLoop);
		static IXAudio2SourceVoice* Play(const wchar_t* soundname);
		static SoundData* GetSoundData(const wchar_t* soundname);

	private:
		SoundManager() = default;
		~SoundManager() = default;

		static IXAudio2* m_xAudio2;
		static IXAudio2MasteringVoice* m_masteringVoice;
		static HRESULT CreateXAudio2();
		static HRESULT CreateMasteringVoice();

		static std::map<std::wstring, SoundData> m_soundDatas;
		static std::list<IXAudio2SourceVoice*> m_sourceVoices;
	};
}