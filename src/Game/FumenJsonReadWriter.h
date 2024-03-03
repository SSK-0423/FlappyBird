#pragma once
#include "picojson.h"
#include "FumenData.h"
#include <vector>

/// <summary>
/// ���ʃf�[�^��Json�t�@�C���ɏ������ށA�܂���Json�t�@�C������ǂݍ��ރN���X
/// </summary>
namespace FlappyBird
{
	class FumenJsonReadWriter
	{
	public:
		FumenJsonReadWriter() = delete;
		~FumenJsonReadWriter() = delete;

		/// <summary>
		/// ���ʂ̃f�[�^��Json�t�@�C���ɏ�������
		/// </summary>
		/// <param name="filePath">�ۑ���</param>
		/// <param name="data">�������ޕ��ʃf�[�^</param>
		static void Write(const std::string& filePath, FumenData& data);

		/// <summary>
		/// ���ʃf�[�^��Json�t�@�C������ǂݍ���
		/// </summary>
		/// <param name="filePath">�ǂݍ��ޕ��ʃt�@�C���p�X</param>
		/// <param name="data">�ǂݎ�����f�[�^�̊i�[��</param>
		static void Read(const std::string& filePath, FumenData& data);
	};
}