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
		// WAV�t�@�C���ǂݍ���
		if (FAILED(DirectX::LoadWAVAudioFromFileEx(filename, wavData, m_wavData)))
		{
			WCHAR message[1024];
			swprintf_s(message, L"wav�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B�Ώۃt�@�C���F %s", filename);
			MessageBoxW(NULL, message, L"�G���[", MB_OK);

			return Utility::RESULT::FAILED;
		}

		// �\�[�X�{�C�X����
		//IXAudio2SourceVoice* sourceVoice;
		if (FAILED(SoundEngine::XAudio2().CreateSourceVoice(&m_sourceVoice, m_wavData.wfx)))
		{
			WCHAR message[1024];
			swprintf_s(message, L"�\�[�X�{�C�X�̐����Ɏ��s���܂����B�Ώۃt�@�C���F %s", filename);
			MessageBoxW(NULL, message, L"�G���[", MB_OK);

			return Utility::RESULT::FAILED;
		}
		//m_sourceVoice.reset(sourceVoice);

		// �\�[�X�{�C�X�ɉ����f�[�^���Z�b�g
		XAUDIO2_BUFFER buffer = { 0 };
		buffer.pAudioData = m_wavData.startAudio;
		// �f�[�^�̏I�[�������t���O �f�[�^�T�C�Y���̃o�b�t�@���Đ�����Ǝ����I�ɒ�~����
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.AudioBytes = m_wavData.audioBytes;

		// TODO:���̑����[�v�Đ��Ȃǂ̐ݒ�

		if (FAILED(m_sourceVoice->SubmitSourceBuffer(&buffer)))
		{
			WCHAR message[1024];
			swprintf_s(message, L"�\�[�X�{�C�X�ւ̉����f�[�^�̃Z�b�g�Ɏ��s���܂����B�Ώۃt�@�C���F %s", filename);
			MessageBoxW(NULL, message, L"�G���[", MB_OK);

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
		// �Đ������ǂ����𔻒肷��K�v������
		// �����Ƀf���[�g����Ă͍���̂ŁA��x�Đ����I���܂ő҂�
		m_sourceVoice->Start();

		// �T�E���h�Ǘ��N���X���쐬���āA�Đ������ǂ����𔻒�A�Ǘ����������悢�̂ł�
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