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
		if (FAILED(CreateMasteringVoice()))
		{
			return Utility::RESULT::FAILED;
		}

		return Utility::RESULT::SUCCESS;
	}
	void SoundEngine::Final()
	{
		if (m_masteringVoice != nullptr)
			m_masteringVoice->DestroyVoice();

		if (m_xAudio2 != nullptr)
			m_xAudio2->Release();

		CoUninitialize();
	}
	IXAudio2& SoundEngine::XAudio2()
	{
		return *m_xAudio2;
	}
	IXAudio2MasteringVoice& SoundEngine::MasteringVoice()
	{
		return *m_masteringVoice;
	}
	HRESULT SoundEngine::CreateXAudio2()
	{
		UINT32 flags = 0;

		HRESULT result = XAudio2Create(&m_xAudio2, flags);

		if (FAILED(result))
		{
			MessageBoxA(NULL, "XAudio2の初期化に失敗", "エラー", MB_OK);
			return result;
		}

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
		HRESULT result = m_xAudio2->CreateMasteringVoice(&m_masteringVoice);

		if (FAILED(result))
		{
			MessageBoxA(NULL, "MasteringVoiceの初期化に失敗", "エラー", MB_OK);
			return result;
		}

		return result;
	}
}

