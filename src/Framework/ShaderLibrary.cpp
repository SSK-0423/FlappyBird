#include "ShaderLibrary.h"

using namespace Utility;
using namespace DX12Wrapper;

namespace Framework
{
	static const TCHAR* SHADER_FOLDER_PASS = L"shaders/";
	static const UINT BUFFSIZE = 128;

	// 静的メンバ変数の実体化
	std::unordered_map<const char*, Shader> ShaderLibrary::m_shaderMap;
	std::unordered_map<SHADERTYPE, const char*> ShaderLibrary::m_shaderTypeMap;

	ShaderLibrary::~ShaderLibrary()
	{
		m_shaderMap.clear();
		m_shaderTypeMap.clear();
	}

	RESULT ShaderLibrary::Init()
	{
		m_shaderTypeMap[SHADERTYPE::VERTEX] = "vs_5_0";
		m_shaderTypeMap[SHADERTYPE::PIXEL] = "ps_5_0";

		// シェーダーの登録
		if (RegistShader(L"Sprite", "SpriteVS", SHADERTYPE::VERTEX, "VSMain") == RESULT::FAILED)
			return RESULT::FAILED;
		if (RegistShader(L"Sprite", "SpritePS", SHADERTYPE::PIXEL, "PSMain") == RESULT::FAILED)
			return RESULT::FAILED;
		if (RegistShader(L"Button", "ButtonVS", SHADERTYPE::VERTEX, "VSMain") == RESULT::FAILED)
			return RESULT::FAILED;
		if (RegistShader(L"Button", "ButtonPS", SHADERTYPE::PIXEL, "PSMain") == RESULT::FAILED)
			return RESULT::FAILED;

		return RESULT::SUCCESS;
	}

	RESULT ShaderLibrary::RegistShader(
		const TCHAR* shaderFileName, const char* shaderTag,
		SHADERTYPE shaderType, const char* entryPointName)
	{
		TCHAR shaderFilePass[BUFFSIZE];
		lstrcpyn(shaderFilePass, SHADER_FOLDER_PASS, BUFFSIZE);
		lstrcat(shaderFilePass, shaderFileName);
		lstrcat(shaderFilePass, L".hlsl");

		Shader shader;
		RESULT result = shader.Create(shaderFilePass, entryPointName, m_shaderTypeMap[shaderType]);
		if (result == RESULT::FAILED) {
			TCHAR errorMessage[BUFFSIZE];
			lstrcpyn(errorMessage, L"ShaderCompileFailed FileName \"", BUFFSIZE);
			lstrcat(errorMessage, shaderFileName);
			lstrcat(errorMessage, L"\"");
			MessageBoxW(NULL, errorMessage, L"エラーメッセージ", MB_OK);

			return RESULT::FAILED;
		}

		m_shaderMap[shaderTag] = shader;

		return RESULT::SUCCESS;
	}
	Shader* ShaderLibrary::GetShader(const char* shaderTag)
	{
		if (m_shaderMap.count(shaderTag) == 0)
			return nullptr;
		return &m_shaderMap[shaderTag];
	}
}