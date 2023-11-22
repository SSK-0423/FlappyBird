#pragma once

#include "DX12Wrapper/Shader.h"
#include "Utility/EngineUtility.h"

namespace Framework {
	enum class SHADERTYPE {
		VERTEX,
		PIXEL
	};

	class ShaderLibrary {
	private:
		ShaderLibrary();
		~ShaderLibrary();
		ShaderLibrary(const ShaderLibrary& inst) = delete;
		void operator=(const ShaderLibrary& inst) = delete;

		std::unordered_map<std::string, DX12Wrapper::Shader> _shaderMap;
		std::unordered_map<SHADERTYPE, const char*> _shaderTypeMap;

	public:
		static ShaderLibrary& Instance();

		void RegistShader(
			const TCHAR* shaderFileName, const std::string& shaderTag,
			SHADERTYPE shaderType, const char* entryPointName = "main");

		DX12Wrapper::Shader* GetShader(const std::string& shaderTag);
	};
}