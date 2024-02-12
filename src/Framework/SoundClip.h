#pragma once

#include <xaudio2.h>
#include "WAVFileReader.h"
#include "IComponent.h"
#include "Subject.h"
#include "NotificationEvent.h"

namespace Framework
{
	/// <summary>
	/// �T�E���h�̍Đ��E��~�Ȃǂ��s���N���X
	/// </summary>
	class SoundClip : public IComponent, IXAudio2VoiceCallback
	{
	public:
		SoundClip(std::shared_ptr<Object> owner);
		~SoundClip();

	public:
		/// <summary>
		/// WAV�`���̃T�E���h��ǂݍ���
		/// </summary>
		/// <param name="filename">wav�t�@�C���ւ̃p�X</param>
		/// <param name="isLoop">���[�v�Đ����邩�ǂ���</param>
		/// <returns>SUCCESS:���� FAILED�F���s</returns>
		Utility::RESULT LoadWavSound(const wchar_t* filename, bool isLoop = false);

		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		/// <summary>
		/// �ǂݍ��񂾃T�E���h���Đ�����
		/// </summary>
		/// <param name="volume">����</param>
		/// <param name="startTimeSec">�Đ��J�n����ʒu(�����E�T�E���h�̒����𒴂��鐔�l�̓G���[�ƂȂ�)</param>
		/// <param name="wait">�Đ����I���܂ő҂��ǂ���</param>
		void Play(float volume = 0.1f, float startTimeSec = 0.f, bool wait = false);

		/// <summary>
		/// �Đ����~�߂�
		/// </summary>
		/// <param name="isPause">�ꎞ��~���ǂ���</param>
		void Stop(bool isPause = false);

		/// <summary>
		/// ���݂̃��[�v�̍Đ����I�������Đ����I������
		/// </summary>
		void ExitLoop();

		/// <summary>
		/// ���ʃZ�b�g
		/// </summary>
		/// <param name="volume">����</param>
		void SetVolume(float volume);

		/// <summary>
		/// �T�E���h�̒�����b�P�ʂŎ擾����
		/// </summary>
		/// <returns>�T�E���h���ǂݍ��܂�Ă��Ȃ��F0.0f ����ȊO�F�T�E���h�̒���(�b)</returns>
		float GetLength();

		/// <summary>
		/// ���݂̍Đ��ʒu���~���b�P�ʂŎ擾����
		/// </summary>
		/// <returns>�T�E���h���ǂݍ��܂�Ă��Ȃ��F-1.0f ����ȊO�F���݂̍Đ��ʒu(�~���b)</returns>
		float GetCurrentPlayTime();

		/// <summary>
		/// �T�E���h�̍Đ��ʒu���w��b������
		/// </summary>
		/// <param name="timeMs">���b����������</param>
		void Move(float timeMs);

		/// <summary>
		/// �ǂݍ��񂾃T�E���h�̍Đ����I�����Ă��邩�ǂ���
		/// </summary>
		/// <returns>true:�I�� false�F�܂�</returns>
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

		INT64 m_samplesPlayed;	    // Move���l�������Đ��ς݃T���v�����@�Đ��ʒu�̎w��ɂ͂��̐��l��p����
		UINT64 m_totalSamples;	    // �T�E���h�̑��T���v����
		INT64 m_startSamplesPlayed;	// Play���Ă΂ꂽ����XAUDIO2_VOICE_STATE����擾�����T���v����
		bool m_isPaused;	        // �ꎞ��~�����ǂ���
		bool m_isEnd;	            // �Đ����I�����Ă��邩�ǂ���
	};
}