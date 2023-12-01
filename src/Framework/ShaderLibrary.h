#pragma once

#include "DX12Wrapper/Shader.h"
#include "Utility/EngineUtility.h"

namespace Framework {
	enum class SHADERTYPE {
		VERTEX,
		PIXEL
	};

	class ShaderLibrary {
	public:
		static Utility::RESULT Init();

		static Utility::RESULT RegistShader(
			const TCHAR* shaderFileName, const char* shaderTag,
			SHADERTYPE shaderType, const char* entryPointName = "main");

		static DX12Wrapper::Shader* GetShader(const char* shaderTag);

	private:
		ShaderLibrary() = default;
		~ShaderLibrary();

		static std::unordered_map<const char*, DX12Wrapper::Shader> m_shaderMap;
		static std::unordered_map<SHADERTYPE, const char*> m_shaderTypeMap;
	};
}