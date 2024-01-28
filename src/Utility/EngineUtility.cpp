#include "EngineUtility.h"
#include <locale>

#pragma comment(lib, "rpcrt4.lib")

namespace Utility
{
	const int EXTENSION_DOT_POINT = 3;

	std::string ReplaceString(std::string str, std::string target, std::string replacement) {
		// �u���������������w�肳��Ă���ꍇ
		if (!target.empty()) {
			std::string::size_type pos = 0;
			// �S�u��
			while ((pos = str.find(target, pos)) != std::string::npos) {
				// �u��
				str.replace(pos, target.length(), replacement);
				pos += replacement.length();
			}
		}

		return str;
	}

	std::string GetExtension(std::string fileName)
	{
		// �g���q���܂܂�Ă��Ȃ���Ή������Ȃ�
		size_t endPos = fileName.length() - 1;
		if (fileName[endPos - EXTENSION_DOT_POINT] != '.') { return ""; }

		return fileName.substr(fileName.length() - EXTENSION_DOT_POINT);
	}

	std::wstring GetExtension(std::wstring fileName)
	{
		// �g���q���܂܂�Ă��Ȃ���Ή������Ȃ�
		size_t endPos = fileName.length() - 1;
		if (fileName[endPos - EXTENSION_DOT_POINT] != '.') { return L""; }

		return fileName.substr(fileName.length() - EXTENSION_DOT_POINT);
	}

	errno_t charToWchar(const char* src, wchar_t* dst, const size_t dstSize) {
		size_t convertedCount;

		// ���{��Ή�
		// ���ꂪ�Ȃ���src�ɓ��{�ꂪ�܂܂�Ă���ꍇ�ɕϊ������s����
		setlocale(LC_ALL, "Japanese");

		errno_t err = mbstowcs_s(&convertedCount, dst, dstSize, src, _TRUNCATE);
		return err;
	}
	std::string GenerateUUID()
	{
		// UUID�𐶐�
		UUID uuid;
		if (UuidCreate(&uuid) != RPC_S_OK)
		{
			MessageBoxA(nullptr, "UUID�̐����Ɏ��s", "�G���[", MB_OK);
		}

		// UUID�𕶎���ɕϊ�
		char* str;
		if (UuidToStringA(&uuid, (RPC_CSTR*)&str) == RPC_S_OUT_OF_MEMORY)
		{
			MessageBoxA(nullptr, "UUID�̕�����ϊ��Ɏ��s", "�G���[", MB_OK);
		}

		std::string result = str;
		// UuidToStringA�Ŋm�ۂ��������������
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