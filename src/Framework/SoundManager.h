#pragma once
#include <xaudio2.h>
#include "WAVFileReader.h"

namespace Framework
{
	/// <summary>
	/// �T�E���h�̐��f�[�^��ێ�����\����
	/// </summary>
	struct SoundData
	{
		std::unique_ptr<uint8_t[]> waveFile;
		DirectX::WAVData waveData;
		XAUDIO2_BUFFER buffer;
	};

	// XAudio2Buffer��ێ�����
	// SouncClip����Đ���v�����ꂽ��ASourceVoice�𐶐����čĐ�����

	/// <summary>
	/// �T�E���h�Đ��ɕK�v�ȃf�[�^���Ǘ��E��������N���X
	/// </summary>
	class SoundManager
	{
	public:
		static Utility::RESULT Init();
		static void DebugDraw();
		static void Reset();
		static void Final();

		/// <summary>
		/// WAV�`���̃T�E���h��ǂݍ���
		/// </summary>
		/// <param name="filename">wav�t�@�C���ւ̃p�X</param>
		/// <param name="isLoop">���[�v�Đ����邩�ǂ���</param>
		/// <returns>RESULT: ���� FAILED�F���s</returns>
		static Utility::RESULT LoadWavSound(const wchar_t* filename, bool isLoop);

		/// <summary>
		/// �����Ŏw�肳�ꂽ�T�E���h���Đ����邽�߂�SourceVoice��Ԃ�
		/// �T�E���h���ǂݍ��ݍς݂̏ꍇ�́A���p�\��SouceVoice��T��or�������ĕԂ�
		/// �ǂݍ��ݍς݂łȂ��ꍇ�́A�ǂݍ��݂��s���ASouceVoice�𐶐����ĕԂ�
		/// </summary>
		/// <param name="soundname">�T�E���h��(wav�t�@�C���ւ̃p�X)</param>
		/// <param name="callback">IXAudio2VoiceCallback�֐������s�����C���X�^���X�ւ̃|�C���^</param>
		/// <returns>�ǂݍ��ݍς݁FSourceVoice �ǂݍ��ݍς݂łȂ��Fnullptr</returns>
		static IXAudio2SourceVoice* Play(const wchar_t* soundname, IXAudio2VoiceCallback* callback);
		
		/// <summary>
		/// �����Ŏw�肳�ꂽ�T�E���h�̖��O�f�[�^���擾����
		/// �ǂݍ��ݍς݂̃T�E���h�̏ꍇ�́A���̃f�[�^��Ԃ�
		/// �ǂݍ��ݍς݂łȂ��ꍇ�́Anullptr��Ԃ�
		/// </summary>
		/// <param name="soundname">�T�E���h��(wav�t�@�C���ւ̃p�X)</param>
		/// <returns>�ǂݍ��ݍς݁FSoundData, �ǂݍ��ݍς݂łȂ��Fnullptr</returns>
		static SoundData* GetSoundData(const wchar_t* soundname);

	private:
		SoundManager() = default;
		~SoundManager() = default;

		static IXAudio2* m_xAudio2;
		static IXAudio2MasteringVoice* m_masteringVoice;
		static HRESULT CreateXAudio2();
		static HRESULT CreateMasteringVoice();

		static std::map<std::wstring, SoundData> m_soundDatas;
		static std::map<std::wstring, std::list<IXAudio2SourceVoice*>> m_sourceVoices;
	};
}