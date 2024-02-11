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
		// �T�E���h�f�[�^�̐ݒ�
		Utility::RESULT LoadWavSound(const wchar_t* filename, bool isLoop = false);

		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		/// <summary>
		/// �T�E���h�Đ�
		/// </summary>
		/// <param name="wait">���̍Đ����I������܂ő҂�</param>
		void Play(float volume = 0.1f, float startTimeSec = 0.f, bool wait = false);

		void Stop(bool isPause = false);

		void ExitLoop();

		float GetLength();
		float GetCurrentPlayTime();
		void Seek(float timeMs);
		bool IsEnd();

		// IXAudio2VoiceCallback
		// �����̃o�b�t�@�[�L���[����f�[�^��ǂݎ�钼�O�ɌĂ΂��
		STDMETHOD_(void, OnVoiceProcessingPassStart)(UINT32 BytesRequired) override;
		// �����̏����p�X���I����������ɌĂ΂��
		STDMETHOD_(void, OnVoiceProcessingPassEnd)() override;
		// �����X�g���[���̍Đ����I�������Ƃ��ɌĂ΂��
		STDMETHOD_(void, OnStreamEnd)() override;
		// �������o�b�t�@�[�̏������J�n���悤�Ƃ��Ă���Ƃ��ɌĂ΂��
		STDMETHOD_(void, OnBufferStart)(void* pBufferContext) override;
		// �������o�b�t�@�[�̏������I�������Ƃ��ɌĂ΂��
		STDMETHOD_(void, OnBufferEnd)(void* pBufferContext) override;
		// ���������[�v�̏I���ʒu�ɒB�����Ƃ��ɌĂ΂��
		STDMETHOD_(void, OnLoopEnd)(void* pBufferContext) override;
		// �����������ɃG���[�����������Ƃ��ɌĂ΂��
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