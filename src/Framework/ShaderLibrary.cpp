#include "ShaderLibrary.h"

using namespace Utility;
using namespace DX12Wrapper;

namespace Framework
{
	static const TCHAR* SHADER_FOLDER_PASS = L"shaders/";
	static const UINT BUFFSIZE = 128;

	ShaderLibrary::ShaderLibrary()
	{
		_shaderTypeMap[SHADERTYPE::VERTEX] = "vs_5_0";
		_shaderTypeMap[SHADERTYPE::PIXEL] = "ps_5_0";

		RegistShader(L"Sprite", "SpriteVS", SHADERTYPE::VERTEX, "VSMain");
		RegistShader(L"Sprite", "SpritePS", SHADERTYPE::PIXEL, "PSMain");
		RegistShader(L"Button", "ButtonVS", SHADERTYPE::VERTEX, "VSMain");
		RegistShader(L"Button", "ButtonPS", SHADERTYPE::PIXEL, "PSMain");
	};
	ShaderLibrary::~ShaderLibrary()
	{
	}
	ShaderLibrary& ShaderLibrary::Instance()
	{
		static ShaderLibrary inst;
		return inst;
	}
	void ShaderLibrary::RegistShader(
		const TCHAR* shaderFileName, const std::string& shaderTag,
		SHADERTYPE shaderType, const char* entryPointName)
	{
		TCHAR shaderFilePass[BUFFSIZE];
		lstrcpyn(shaderFilePass, SHADER_FOLDER_PASS, BUFFSIZE);
		lstrcat(shaderFilePass, shaderFileName);
		lstrcat(shaderFilePass, L".hlsl");

		Shader shader;
		RESULT result = shader.Create(shaderFilePass, entryPointName, _shaderTypeMap[shaderType]);
		if (result == RESULT::FAILED) {
			TCHAR errorMessage[BUFFSIZE];
			lstrcpyn(errorMessage, L"ShaderCompileFailed FileName \"", BUFFSIZE);
			lstrcat(errorMessage, shaderFileName);
			lstrcat(errorMessage, L"\"");
			MessageBoxW(NULL, errorMessage, L"エラーメッセージ", MB_OK);
		}

		_shaderMap[shaderTag] = shader;
	}
	Shader* ShaderLibrary::GetShader(const std::string& shaderTag)
	{
		if (_shaderMap.count(shaderTag) == 0)
			return nullptr;
		return &_shaderMap[shaderTag];
	}
}