#pragma once

#include "DX12Wrapper/Shader.h"
#include "Utility/EngineUtility.h"

namespace Framework {
	/// <summary>
	/// シェーダーの種類
	/// </summary>
	enum class SHADERTYPE {
		VERTEX,
		PIXEL
	};

	/// <summary>
	/// シェーダーを事前に読み込んで管理するクラス
	/// </summary>
	class ShaderLibrary {
	public:
		static Utility::RESULT Init();

		/// <summary>
		/// ライブラリへシェーダーを登録する
		/// </summary>
		/// <param name="shaderFileName">シェーダーファイルネーム</param>
		/// <param name="shaderTag">シェーダーの識別子</param>
		/// <param name="shaderType">シェーダーの種類</param>
		/// <param name="entryPointName">エントリポイント名</param>
		/// <returns>成功：SUCCESS 失敗:FAILED</returns>
		static Utility::RESULT RegistShader(
			const TCHAR* shaderFileName, const char* shaderTag,
			SHADERTYPE shaderType, const char* entryPointName = "main");

		/// <summary>
		/// ライブラリからシェーダーを取得する
		/// </summary>
		/// <param name="shaderTag">取得したいシェーダーの識別子</param>
		/// <returns>存在する場合：シェーダー　しない場合：nullptr</returns>
		static DX12Wrapper::Shader* GetShader(const char* shaderTag);

	private:
		ShaderLibrary() = default;
		~ShaderLibrary();

		static std::unordered_map<const char*, DX12Wrapper::Shader> m_shaderMap;
		static std::unordered_map<SHADERTYPE, const char*> m_shaderTypeMap;
	};
}