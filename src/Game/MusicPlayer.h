#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	/// <summary>
	/// �Ȃ̍Đ����Ǘ�����R���|�[�l���g
	/// </summary>
	class MusicPlayer : public Framework::IComponent
	{
	public:
		MusicPlayer(std::shared_ptr<Framework::Object> owner);
		~MusicPlayer();

		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		/// <summary>
		/// �Ȃ�ǂݍ���
		/// </summary>
		/// <param name="musicPath">�Ȃ̃t�@�C���p�X</param>
		/// <param name="isLoop">���[�v�Đ����邩�ǂ���</param>
		void Load(const std::string& musicPath, bool isLoop);
		void Play(float startTime = 0.f);
		void Stop();

		/// <summary>
		/// �Ȃ̒�����b�P�ʂŎ擾����
		/// </summary>
		/// <returns>�Ȃ̒���(�b)</returns>
		float GetMusicLength();

		/// <summary>
		/// ���݂̍Đ����Ԃ��~���b�P�ʂŎ擾����
		/// </summary>
		/// <returns>���݂̍Đ�����</returns>
		float GetCurrentPlayTimeMs();

		/// <summary>
		/// �Ȃ̍Đ��ʒu�𓮂���
		/// </summary>
		/// <param name="timeSec">���b����������</param>
		void Move(float timeSec);

		bool IsPlaying();

		void SetVolume(float volume);
		
		void SetBPM(float bpm);
		float GetBPM();

		/// <summary>
		/// �Ȃ̔��q��ݒ肷��
		/// </summary>
		/// <param name="beat"></param>
		void SetBeat(float beat);

		/// <summary>
		/// �Ȃ̔��q���擾����
		/// </summary>
		/// <returns></returns>
		float GetBeat();

		Framework::Subject<Framework::NotificationEvent> OnMusicEnd;	// �Ȃ��Đ����I�������Ƃ��ɒʒm����C�x���g

	private:
		Framework::SoundClip* m_music;
		wchar_t m_musicPath[256];
		bool m_isPlaying;
		float m_beat;
		float m_bpm;
	};
}