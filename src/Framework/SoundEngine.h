#pragma once

#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

namespace Framework
{
	class SoundEngine
	{
	public:
		static Utility::RESULT Init();

		static IXAudio2& GetXAudio2();

	private:
		SoundEngine() = default;
		~SoundEngine() = default;

		static IXAudio2* m_xAudio2;
		static IXAudio2MasteringVoice* m_masteringVoice;
		static HRESULT CreateXAudio2();
		static HRESULT CreateMasteringVoice();
	};
}