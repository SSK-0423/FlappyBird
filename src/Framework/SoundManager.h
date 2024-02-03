#pragma once
#include <xaudio2.h>
#include "WAVFileReader.h"

namespace Framework
{
	// XAudio2Buffer‚ğ•Û‚·‚é
	// SouncClip‚©‚çÄ¶‚ğ—v‹‚³‚ê‚½‚çASourceVoice‚ğ¶¬‚µ‚ÄÄ¶‚·‚é
	class SoundManager
	{
	public:
		static Utility::RESULT Init();
		static void Reset();
		static void Final();
		static Utility::RESULT LoadWavSound(const wchar_t* filename, bool isLoop);
		static IXAudio2SourceVoice* Play(const wchar_t* soundname);
		static float GetSoundLength(const wchar_t* soundname);

	private:
		SoundManager() = default;
		~SoundManager() = default;

		static IXAudio2* m_xAudio2;
		static IXAudio2MasteringVoice* m_masteringVoice;
		static HRESULT CreateXAudio2();
		static HRESULT CreateMasteringVoice();

		struct SoundData
		{
			std::unique_ptr<uint8_t[]> waveFile;
			DirectX::WAVData waveData;
			XAUDIO2_BUFFER buffer;
		};
		static std::map<std::wstring, SoundData> m_soundDatas;
		static std::list<IXAudio2SourceVoice*> m_sourceVoices;
	};
}