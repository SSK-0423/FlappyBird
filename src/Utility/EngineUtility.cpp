#include "EngineUtility.h"
#include <locale>

#pragma comment(lib, "rpcrt4.lib")

namespace Utility
{
	const int EXTENSION_DOT_POINT = 3;

	std::string ReplaceString(std::string str, std::string target, std::string replacement) {
		// 置換したい文字が指定されている場合
		if (!target.empty()) {
			std::string::size_type pos = 0;
			// 全置換
			while ((pos = str.find(target, pos)) != std::string::npos) {
				// 置換
				str.replace(pos, target.length(), replacement);
				pos += replacement.length();
			}
		}

		return str;
	}

	std::string GetExtension(std::string fileName)
	{
		// 拡張子が含まれていなければ何もしない
		size_t endPos = fileName.length() - 1;
		if (fileName[endPos - EXTENSION_DOT_POINT] != '.') { return ""; }

		return fileName.substr(fileName.length() - EXTENSION_DOT_POINT);
	}

	std::wstring GetExtension(std::wstring fileName)
	{
		// 拡張子が含まれていなければ何もしない
		size_t endPos = fileName.length() - 1;
		if (fileName[endPos - EXTENSION_DOT_POINT] != '.') { return L""; }

		return fileName.substr(fileName.length() - EXTENSION_DOT_POINT);
	}

	errno_t charToWchar(const char* src, wchar_t* dst, const size_t dstSize) {
		size_t convertedCount;

		// 日本語対応
		// これがないとsrcに日本語が含まれている場合に変換が失敗する
		setlocale(LC_ALL, "Japanese");

		errno_t err = mbstowcs_s(&convertedCount, dst, dstSize, src, _TRUNCATE);
		return err;
	}
	std::string GenerateUUID()
	{
		// UUIDを生成
		UUID uuid;
		if (UuidCreate(&uuid) != RPC_S_OK)
		{
			MessageBoxA(nullptr, "UUIDの生成に失敗", "エラー", MB_OK);
		}

		// UUIDを文字列に変換
		char* str;
		if (UuidToStringA(&uuid, (RPC_CSTR*)&str) == RPC_S_OUT_OF_MEMORY)
		{
			MessageBoxA(nullptr, "UUIDの文字列変換に失敗", "エラー", MB_OK);
		}

		std::string result = str;
		// UuidToStringAで確保したメモリを解放
		RpcStringFreeA((RPC_CSTR*)&str);

		return result;
	}
	std::vector<std::string> GetFilePathsInDirectoryWithExtension(const std::string& directoryPath, const std::string& extension)
	{
		std::vector<std::string> filePaths;

		for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
			if (entry.path().extension() == extension) {
				filePaths.push_back(entry.path().string());
			}
		}

		return filePaths;
	}
}