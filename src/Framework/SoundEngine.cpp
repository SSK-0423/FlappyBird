#include "pch.h"
#include "SoundEngine.h"

namespace Framework
{
	// 静的メンバ変数の実体化
	IXAudio2* SoundEngine::m_xAudio2 = nullptr;
	IXAudio2MasteringVoice* SoundEngine::m_masteringVoice = nullptr;

	Utility::RESULT Framework::SoundEngine::Init()
	{
		CoInitializeEx(nullptr, COINIT_MULTITHREADED);

		if (FAILED(CreateXAudio2()))
		{
			return Utility::RESULT::FAILED;
		}

		return Utility::RESULT::SUCCESS;
	}
	IXAudio2& SoundEngine::GetXAudio2()
	{
		return *m_xAudio2;
	}
	HRESULT SoundEngine::CreateXAudio2()
	{
		UINT32 flags = 0;

		HRESULT result = XAudio2Create(&m_xAudio2, flags);

		// デバッグ設定(Win8以降のみ)
#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/) && defined(_DEBUG)
		XAUDIO2_DEBUG_CONFIGURATION debug = { 0 };
		debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
		debug.BreakMask = XAUDIO2_LOG_ERRORS;
		m_xAudio2->SetDebugConfiguration(&debug, 0);
#endif

		return result;
	}
	HRESULT SoundEngine::CreateMasteringVoice()
	{
		return E_NOTIMPL;
	}
}

