#pragma once
#include "picojson.h"
#include "FumenData.h"
#include <vector>

/// <summary>
/// 譜面データをJsonファイルに書き込む、またはJsonファイルから読み込むクラス
/// </summary>
namespace FlappyBird
{
	class FumenJsonReadWriter
	{
	public:
		FumenJsonReadWriter() = delete;
		~FumenJsonReadWriter() = delete;

		/// <summary>
		/// 譜面のデータをJsonファイルに書き込む
		/// </summary>
		/// <param name="filePath">保存先</param>
		/// <param name="data">書き込む譜面データ</param>
		static void Write(const std::string& filePath, FumenData& data);

		/// <summary>
		/// 譜面データをJsonファイルから読み込む
		/// </summary>
		/// <param name="filePath">読み込む譜面ファイルパス</param>
		/// <param name="data">読み取ったデータの格納先</param>
		static void Read(const std::string& filePath, FumenData& data);
	};
}