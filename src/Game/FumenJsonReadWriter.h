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
		/// <param name="filePath"></param>
		/// <param name="data"></param>
		static void Write(const std::string& filePath, const FumenData& data);

		/// <summary>
		/// ���ʃf�[�^��Json�t�@�C������ǂݍ���
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="data"></param>
		static void Read(const std::string& filePath, FumenData& data);
	};
}