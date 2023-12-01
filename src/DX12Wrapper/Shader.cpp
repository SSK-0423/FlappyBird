#include "Shader.h"
#include <d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

using namespace Utility;

namespace DX12Wrapper
{
	HRESULT Shader::CompileShader(
		const TCHAR* shaderFilePass, const char* entoryPointName, const char* shaderTypeAndVersion)
	{
#ifdef _DEBUG
		// シェーダーのコンパイル
		HRESULT result = D3DCompileFromFile(
			shaderFilePass,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entoryPointName, shaderTypeAndVersion,
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&m_shaderBlob, &m_errorBlob);
#else
		// シェーダーのコンパイル
		HRESULT result = D3DCompileFromFile(
			shaderFilePass,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entoryPointName, shaderTypeAndVersion,
			D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&m_shaderBlob, &m_errorBlob);
#endif // _DEBUG

		if (FAILED(result)) {
			if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)) {
				::OutputDebugStringA("ファイルが見当たりません");
			}
			else {
				std::string errstr;	// エラーメッセージ受け取り用string
				errstr.resize(m_errorBlob->GetBufferSize()); // 必要なサイズ分確保
				// データコピー
				std::copy_n((char*)m_errorBlob->GetBufferPointer(),
					m_errorBlob->GetBufferSize(),
					errstr.begin());
				errstr += "\n";

				::OutputDebugStringA(errstr.c_str());
			}
		}

		return result;
	}

	RESULT Shader::Create(
		const TCHAR* shaderFilePass, const char* entoryPointName, const char* shaderTypeAndVersion)
	{
		// シェーダーをコンパイル
		if (FAILED(CompileShader(shaderFilePass, entoryPointName, shaderTypeAndVersion))) {
			return RESULT::FAILED;
		}

		return RESULT::SUCCESS;
	}

	RESULT Shader::Create(const ShaderData& shaderData)
	{
		RESULT result =
			Create(shaderData.shaderFilePass, shaderData.entoryPointName, shaderData.shaderType);
		if (result == RESULT::FAILED) { return result; }

		return RESULT::SUCCESS;
	}
}
