#include "SoundManager.h"


#include "Editor.h"

namespace Framework
{
	// �ÓI�����o�ϐ��̎��̉�
	IXAudio2* SoundManager::m_xAudio2 = nullptr;
	IXAudio2MasteringVoice* SoundManager::m_masteringVoice = nullptr;
	std::map<std::wstring, SoundData> SoundManager::m_soundDatas;
	std::list<IXAudio2SourceVoice*> SoundManager::m_sourceVoices;

	Utility::RESULT SoundManager::Init()
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
	void SoundManager::Reset()
	{
		for (auto source : m_sourceVoices)
		{
			source->Stop();
			source->DestroyVoice();
		}
		m_sourceVoices.clear();

		m_soundDatas.clear();
	}
	void SoundManager::Final()
	{
		m_soundDatas.clear();

		for (auto source : m_sourceVoices)
		{
			source->Stop();
			source->DestroyVoice();
		}
		m_sourceVoices.clear();

		if (m_masteringVoice != nullptr)
			m_masteringVoice->DestroyVoice();

		if (m_xAudio2 != nullptr)
			m_xAudio2->Release();

		CoUninitialize();
	}

	HRESULT SoundManager::CreateXAudio2()
	{
		UINT32 flags = 0;

		HRESULT result = XAudio2Create(&m_xAudio2, flags);

		if (FAILED(result))
		{
			MessageBoxA(NULL, "XAudio2�̏������Ɏ��s", "�G���[", MB_OK);
			return result;
		}

		// �f�o�b�O�ݒ�(Win8�ȍ~�̂�)
#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/) && defined(_DEBUG)
		XAUDIO2_DEBUG_CONFIGURATION debug = { 0 };
		debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
		debug.BreakMask = XAUDIO2_LOG_ERRORS;
		m_xAudio2->SetDebugConfiguration(&debug, 0);
#endif

		return result;
	}
	HRESULT SoundManager::CreateMasteringVoice()
	{
		HRESULT result = m_xAudio2->CreateMasteringVoice(&m_masteringVoice);

		if (FAILED(result))
		{
			MessageBoxA(NULL, "MasteringVoice�̏������Ɏ��s", "�G���[", MB_OK);
			return result;
		}

		return result;
	}

	Utility::RESULT SoundManager::LoadWavSound(const wchar_t* filename, bool isLoop)
	{
		// ���ɓǂݍ��܂�Ă��邩�`�F�b�N
		if (m_soundDatas.find(filename) != m_soundDatas.end())
		{
			return Utility::RESULT::SUCCESS;
		}

		// WAV�t�@�C���ǂݍ���
		SoundData soundData;
		if (FAILED(DirectX::LoadWAVAudioFromFileEx(filename, soundData.waveFile, soundData.waveData)))
		{
			WCHAR message[1024];
			swprintf_s(message, L"wav�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B�Ώۃt�@�C���F %s", filename);
			MessageBoxW(NULL, message, L"�G���[", MB_OK);

			return Utility::RESULT::FAILED;
		}
		// �o�b�t�@�[����
		soundData.buffer = { 0 };
		soundData.buffer.pAudioData = soundData.waveData.startAudio;
		soundData.buffer.AudioBytes = soundData.waveData.audioBytes;
		soundData.buffer.Flags = XAUDIO2_END_OF_STREAM;
		if (isLoop)
		{
			soundData.buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
		}

		// �}�b�v�ɒǉ�
		m_soundDatas[filename] = std::move(soundData);
	}
	IXAudio2SourceVoice* SoundManager::Play(const wchar_t* soundname)
	{
#ifdef _DEBUG
		// �\�[�X�{�C�X�̑�����\��
		Editor::DebugLog("SourceVoice Count: %d", m_sourceVoices.size());
#endif // _DEBUG

		// �T�E���h���ǂݍ��ݍς݂��`�F�b�N����
		if (m_soundDatas.find(soundname) == m_soundDatas.end())
		{
			return nullptr;
		}

		IXAudio2SourceVoice* sourceVoice = nullptr;

		// �g�p�\�ȃ\�[�X�{�C�X��T��
		for (auto source : m_sourceVoices)
		{
			XAUDIO2_VOICE_STATE state;
			source->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
			if (state.BuffersQueued == 0)
			{
				sourceVoice = source;
				break;
			}
		}

		// �g�p�\�ȃ\�[�X�{�C�X���Ȃ���΍쐬����
		if (sourceVoice == nullptr)
		{
			HRESULT result = m_xAudio2->CreateSourceVoice(&sourceVoice, m_soundDatas[soundname].waveData.wfx);
			if (FAILED(result))
			{
				MessageBoxA(NULL, "SourceVoice�̍쐬�Ɏ��s", "�G���[", MB_OK);
				return nullptr;
			}
			m_sourceVoices.push_back(sourceVoice);
		}

		return sourceVoice;
	}
	SoundData* SoundManager::GetSoundData(const wchar_t* soundname)
	{
		// �T�E���h���ǂݍ��ݍς݂��`�F�b�N����
		if (m_soundDatas.find(soundname) == m_soundDatas.end())
		{
			return nullptr;
		}

		return &m_soundDatas[soundname];
	}
}